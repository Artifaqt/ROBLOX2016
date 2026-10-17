/**
 * NAMEHEREDocManager.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "NAMEHEREDocManager.h"

// Qt Headers
#include <QString>
#include <QMessageBox>

// NAMEHERE Headers
#include "Script/LuaSourceContainer.h"
#include "Util/standardout.h"
#include "Util/ContentId.h"
#include "V8DataModel/PluginManager.h"

// NAMEHERE Studio Headers
#include "DocDockManager.h"
#include "DocTabManager.h"
#include "LuaSourceBuffer.h"
#include "NAMEHEREMainWindow.h"
#include "NAMEHEREGameExplorer.h"
#include "NAMEHEREIDEDoc.h"
#include "NAMEHEREScriptDoc.h"
#include "NAMEHEREWebDoc.h"
#include "NAMEHEREObjectBrowserDoc.h"
#include "UpdateUIManager.h"
#include "ScriptTextEditor.h"

NAMEHEREDocManager& NAMEHEREDocManager::Instance()
{
	static NAMEHEREDocManager singleInstance;
	return singleInstance;
}

NAMEHEREDocManager::NAMEHEREDocManager()
: m_pMainWindow(NULL)
, m_pCurrentDoc(NULL)
, m_pPlayDoc(NULL)
, m_pObjectBrowser(NULL)
, m_pWebDoc(NULL)
, m_DocTabManager(NULL)
, m_DocDockManager(NULL)
{
}

NAMEHEREDocManager::~NAMEHEREDocManager()
{
}

/**
 * Only call initialize once.
 */
void NAMEHEREDocManager::initialize(NAMEHEREMainWindow& mainWindow)
{
    RBXASSERT(!m_pMainWindow);

	m_pMainWindow = &mainWindow;
    m_DocTabManager = new DocTabManager(mainWindow);
    m_DocDockManager = new DocDockManager(mainWindow);
}

void NAMEHEREDocManager::shutDown()
{
    m_pCurrentDoc = NULL;
    m_pPlayDoc = NULL;
	m_KeyDocMap.clear();

    delete m_DocTabManager;
    delete m_DocDockManager;

    if(m_pObjectBrowser)
        delete m_pObjectBrowser;

    if(m_pWebDoc)
        delete m_pWebDoc;

    m_DocTabManager = NULL;
    m_DocDockManager = NULL;
    m_pMainWindow = NULL;
}

/**
 * Attempt to close all docs.
 *  Any failure to close aborts the operation.
 *  
 * @return true if all docs closed successfully
 */
bool NAMEHEREDocManager::requestCloseAllDocs()
{
    while ( !m_KeyDocMap.empty() )
    {
        RBX::BaldPtr<INAMEHEREDoc> doc = m_KeyDocMap.begin().value();
        if ( !m_pMainWindow->requestDocClose(doc) )
            return false;
    }

    return true;
}

/**
 * Attempts to save and close all docs.
 *  Any failure to save will abort the process.
 *  
 * @return true if all docs were saved successfully
 */
bool NAMEHEREDocManager::requestCloseAndSaveAllDocs()
{
    while ( !m_KeyDocMap.empty() )
    {
        RBX::BaldPtr<INAMEHEREDoc> doc = m_KeyDocMap.begin().value();
        if ( !m_pMainWindow->fileSave(doc) )
            return false;
        removeDoc(*doc);
    }

    return true;
}

/**
 * Closes all docs without saving.
 */
void NAMEHEREDocManager::closeAllDocs()
{
    while ( !m_KeyDocMap.empty() )
    {
        RBX::BaldPtr<INAMEHEREDoc> doc = m_KeyDocMap.begin().value();
        removeDoc(*doc);
    }
}

/**
 * Registers a script doc.
 *  Used for attempting to reopen scripts.
 */
void NAMEHEREDocManager::registerOpenedScriptDoc(NAMEHEREScriptDoc& doc, LuaSourceBuffer script)
{
	RBXASSERT(!m_OpenScriptDocs.contains(&doc));
	m_OpenScriptDocs.push_back(&doc);
}

/**
 * Unregisters a script doc.
 *  Used for attempting to reopen scripts.
 */
void NAMEHEREDocManager::deregisterScriptDoc(NAMEHEREScriptDoc& doc)
{
	RBXASSERT(m_OpenScriptDocs.contains(&doc));
	m_OpenScriptDocs.removeAll(&doc);
}


/**
 * Returns a handle to the ObjectBrowser, or NULL if it hasnt been created.
 */
NAMEHEREObjectBrowserDoc* NAMEHEREDocManager::getObjectBrowserDoc()
{
	return m_pObjectBrowser;
}

NAMEHEREScriptDoc* NAMEHEREDocManager::findOpenScriptDoc(LuaSourceBuffer script)
{
	for (tScriptDocs::iterator itr = m_OpenScriptDocs.begin(); itr != m_OpenScriptDocs.end(); ++itr)
	{
		if ((*itr)->getCurrentScript() == script)
		{
			return *itr;
		}
	}
	return NULL;
}

/**
 * Creates a doc or returns an existing doc of the given type.
 *  The Object Browser and Start Page get created only once.
 */
INAMEHEREDoc* NAMEHEREDocManager::getOrCreateDoc(INAMEHEREDoc::RBXDocType docType)
{
	RBX::BaldPtr<INAMEHEREDoc> pDoc = NULL;
	
    switch ( docType )
    {
        case INAMEHEREDoc::IDE:
        {
		    m_pPlayDoc = new NAMEHEREIDEDoc(m_pMainWindow);
            pDoc = m_pPlayDoc;
            break;
        }
        case INAMEHEREDoc::SCRIPT:
        {
		    pDoc = new NAMEHEREScriptDoc();
            break;
        }
        case INAMEHEREDoc::BROWSER:
	    {
		    if ( !m_pWebDoc )
			    m_pWebDoc = new NAMEHEREWebDoc(tr("Start Page"), "StartPage");
			pDoc = m_pWebDoc;
            break;
	    }
        case INAMEHEREDoc::OBJECTBROWSER:
	    {
		    if ( !m_pObjectBrowser )
			    m_pObjectBrowser = new NAMEHEREObjectBrowserDoc();
			pDoc = m_pObjectBrowser;
            break;
        }
        default:
            RBXASSERT(false);
	}
	
	return pDoc;
}

/**
 * Attempt to open a script document.
 *  If the script is already open, the doc will be returned.
 *  If the script is not already open, a new doc will be returned.
 */
NAMEHEREScriptDoc* NAMEHEREDocManager::openDoc(LuaSourceBuffer script)
{
	if (script.empty())
		return NULL;

	boost::scoped_ptr<RBX::DataModel::LegacyLock> lock;

	// Double click scripts in game explorer will attempt to open doc, only open
	// script doc if we have a play doc.
	if (m_pPlayDoc == NULL)
	{
		return NULL;
	}

	// NAMEHEREScriptDoc::setScript needs a write lock
	lock.reset(new RBX::DataModel::LegacyLock(m_pPlayDoc->getDataModel(), RBX::DataModelJob::Write));

	// check if it is a linked script, and if so substitute opening linked source
	if (shared_ptr<RBX::LuaSourceContainer> lsc = script.toInstance())
	{
		if (!lsc->getScriptId().isNull())
		{
			script = LuaSourceBuffer::fromContentId(lsc->getScriptId());
		}
	}

	if (!script.sourceAvailable())
	{
		// release the lock before entering message box event loop
		lock.reset();
		QMessageBox mb;
		mb.setText("Unable to load script. If the script uses LinkedSource, the "
			"LinkedSource may have been renamed or removed.");
		mb.exec();
		return NULL;
	}

	// If the doc for this script already exists, then return the same
	if (NAMEHEREScriptDoc* doc = findOpenScriptDoc(script))
	{
		setCurrentDoc(doc);
		return doc;
	}

	// Create new script document
	NAMEHEREScriptDoc* pDoc = new NAMEHEREScriptDoc();
	if ( !pDoc->open(m_pMainWindow,"") )
	{
		RBX::StandardOut::singleton()->printf(RBX::MESSAGE_INFO, "Error in opening script");
		pDoc->close();
		delete pDoc;
		return NULL;
	}

    configureDocWidget(*pDoc);
	pDoc->setScript(shared_from(getPlayDoc()->getDataModel()), script);

	return pDoc;
}

void NAMEHEREDocManager::openDoc(shared_ptr<RBX::Instance> script, int lineNumber)
{
	if (NAMEHEREScriptDoc* scriptDoc = openDoc(LuaSourceBuffer::fromInstance(script)))
		scriptDoc->getTextEditor()->moveCursorToLine(lineNumber);
}

/**
 * Requests a script doc to be closed.
 */
void NAMEHEREDocManager::closeDoc(const LuaSourceBuffer& script)
{
    RBXASSERT(!script.empty());

	if (NAMEHEREScriptDoc* pDoc = findOpenScriptDoc(script))
	{
		m_pMainWindow->requestDocClose(*pDoc);
	}
}

/**
 * Adds the doc to the doc tab manager.
 */
void NAMEHEREDocManager::configureDocWidget(INAMEHEREDoc& doc)
{
    const RBX::BaldPtr<QWidget> widget = doc.getViewer();
    RBXASSERT(widget);

    const QString keyName = doc.keyName();
    RBXASSERT(!keyName.isEmpty());
    
	if (!m_KeyDocMap.contains(keyName))
	{
		m_KeyDocMap[keyName] = &doc;
		m_DocTabManager->addDoc(doc);
	}
    setCurrentDoc(&doc);
}

/**
 * Removes a doc from the tab or dock manager.
 */
void NAMEHEREDocManager::removeDoc(INAMEHEREDoc& doc)
{
    m_KeyDocMap.remove(doc.keyName());

	if ( &doc == m_pCurrentDoc )
    {
        setCurrentDoc(NULL);
        m_pCurrentDoc = NULL;
    }

	if ( &doc == m_pPlayDoc )
    {
        restoreCentralWidget();

		tScriptDocs copyOfScriptDocs = m_OpenScriptDocs;
		for (tScriptDocs::iterator itr = copyOfScriptDocs.begin();
			itr != copyOfScriptDocs.end(); ++itr)
		{
			m_pMainWindow->requestDocClose(*itr);
		}
		RBXASSERT(m_OpenScriptDocs.empty());

		m_pPlayDoc = NULL;
    }

    if (m_DocTabManager && !m_DocTabManager->removeDoc(doc) )
    {
        m_DocDockManager->removeDoc(doc);
        setCurrentDoc(m_DocTabManager->getCurrentDoc());
    }

    doc.close();
    updateWindowMenu();
}

/**
 * Sets the current doc.
 *  The doc may be NULL.  The old doc is deactivated.
 */
void NAMEHEREDocManager::setCurrentDoc(INAMEHEREDoc* doc)
{
    if ( doc == m_pCurrentDoc )
	{
		if (m_pCurrentDoc)
			m_pCurrentDoc->activate();
        return;
	}

	bool docTypeChanged = (m_pCurrentDoc ? m_pCurrentDoc->docType() : -1) != (doc ? doc->docType() : -1);

	if ( m_pCurrentDoc )
	{
		m_pCurrentDoc->deActivate();
		if (docTypeChanged)
			m_pCurrentDoc->updateUI(false);
	}

    m_pCurrentDoc = doc;

    if ( m_pCurrentDoc )
	{
        m_pCurrentDoc->activate();
		if (docTypeChanged)
			m_pCurrentDoc->updateUI(true);
	}
    else
	{
        RBX::PluginManager::singleton()->setCurrentDataModel(NULL);
	}

    UpdateUIManager::Instance().updateToolBars();
	m_pMainWindow->updateWindowTitle();

    if ( !m_DocTabManager->setCurrentDoc(*doc) )
    {
        m_pMainWindow->setFocus();
        m_DocDockManager->setCurrentDoc(*doc);
    }
	else if (m_pCurrentDoc)
	{
		m_pCurrentDoc->getViewer()->setFocus();
	}

	updateWindowMenu();
	Q_EMIT currentDocChanged();
}

/**
 * Updates the list of open documents in the Window menu.
 *  The current open doc is marked with a check.
 */
void NAMEHEREDocManager::updateWindowMenu()
{
	if (!m_pMainWindow)
		return;

    int count = 0;
    tKeyDocMap::iterator iter = m_KeyDocMap.begin();
    while ( iter != m_KeyDocMap.end() && count < NAMEHEREMainWindow::MAX_DOC_WINDOWS )
    {
        RBX::BaldPtr<INAMEHEREDoc> doc = iter.value();
		QString text = QObject::tr("&%1 %2").arg(count + 1).arg(doc->displayName());
		m_pMainWindow->currentOpenedfiles[count]->setText(text);
		m_pMainWindow->currentOpenedfiles[count]->setData(count);
		m_pMainWindow->currentOpenedfiles[count]->setVisible(true);

        bool selected = (doc == m_pCurrentDoc);
        m_pMainWindow->currentOpenedfiles[count]->setChecked(selected); 

        ++iter;
        ++count;
    }

    while ( count < NAMEHEREMainWindow::MAX_DOC_WINDOWS )
    {
        m_pMainWindow->currentOpenedfiles[count]->setVisible(false);
        ++count;
    }
}

/**
 * Callback for user clicking on a doc in the Window menu.
 */
void NAMEHEREDocManager::onActivateWindow()
{
    const RBX::BaldPtr<QAction> action = qobject_cast<QAction*>(sender());
	int index = action->data().toInt();
    activateDoc(index);
    updateWindowMenu();
}

/**
 * Activate a doc at the given index.
 *  Used by onActivateWindow.
 */
void NAMEHEREDocManager::activateDoc(int index)
{
    tKeyDocMap::iterator iter = m_KeyDocMap.begin();
    iter = iter + index;
    if ( iter != m_KeyDocMap.end() )
        setCurrentDoc(iter.value());
}

/**
 * Changes the title of a doc.
 *  If the doc is a tab, the tab name is changed.  If the doc is a dock widget, the window
 *  title is changed.
 */
void NAMEHEREDocManager::setDocTitle(INAMEHEREDoc& doc,const QString& displayName,const QString& fileName,const QIcon& icon)
{
    const QString oldKey = m_KeyDocMap.key(&doc);
    RBXASSERT(!oldKey.isEmpty());

    m_KeyDocMap.remove(oldKey);
	m_KeyDocMap[doc.keyName()] = &doc;

    if ( !m_DocTabManager->renameDoc(doc,displayName,fileName,icon) )
        m_DocDockManager->renameDoc(doc,displayName,fileName);

    updateWindowMenu();
    m_pMainWindow->updateWindowTitle();
}

/**
 * Detaches the given doc from the tab manager.
 *  Optionally begins dragging the title bar to move the window.
 */
void NAMEHEREDocManager::detachDoc(INAMEHEREDoc& doc,bool dragging)
{
    if ( m_DocTabManager->removeDoc(doc) )
    {
        m_DocDockManager->addDoc(doc);
        if ( dragging )
            m_DocDockManager->startDrag(doc);
        m_DocDockManager->setCurrentDoc(doc);
    }
}

/**
 * Attaches a dock widget to the tab manager.
 */
void NAMEHEREDocManager::attachDoc(INAMEHEREDoc& doc, int index)
{
    if ( m_DocDockManager->removeDoc(doc) )
        m_DocTabManager->addDoc(doc, index);
}

/**
 * Sets the current state of the mouse hover over.
 *  When a dock widget is hovered over the tab widget while dragging,
 *  the tab widget shows if the dock can be reattached.
 */
void NAMEHEREDocManager::setDockHoverOverPos(const QPoint& globalPos)
{
    m_DocTabManager->setDockHoverOverPos(globalPos);
}

/**
 * Attempts to attach a dock widget.
 *  If the mouse position is inside the region allowable for reattachment,
 *  the doc is reattached to the tab bar.
 * 
 * @param   doc         doc to attach
 * @param   worldPos    position the mouse button was released
 */
bool NAMEHEREDocManager::attemptAttach(INAMEHEREDoc& doc,const QPoint& globalPos)
{
    return m_DocTabManager->attemptAttach(doc,globalPos);
}

/**
 * Sets the central widget for the main window to just show the IDE 3D view.
 */
void NAMEHEREDocManager::setPlayDocCentralWidget()
{
    RBXASSERT(m_pPlayDoc);

    QWidget* centralWidget = m_pMainWindow->centralWidget();
    if ( centralWidget != m_pPlayDoc->getViewer() )
    {
       if ( !m_DocDockManager->removeDoc(*m_pPlayDoc) )
            m_DocTabManager->removeDoc(*m_pPlayDoc);

        m_pMainWindow->setCentralWidget(m_pPlayDoc->getViewer());
        QApplication::removePostedEvents(centralWidget);            // don't allow to be deleted
        m_pPlayDoc->getViewer()->show();

        setCurrentDoc(m_pPlayDoc);
    }
}

/**
 * Resets the central widget for the main window back to the tab manager.
 */
void NAMEHEREDocManager::restoreCentralWidget()
{
    m_DocTabManager->restoreAsCentralWidget();
}
