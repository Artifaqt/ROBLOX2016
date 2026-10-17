/**
 * NAMEHEREDocManager.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QObject>
#include <QMap>
#include <QIcon>

// 3rd Party Headers
#include "boost/shared_ptr.hpp"

// NAMEHERE Headers
#include "rbx/BaldPtr.h"
#include "V8DataModel/DataModel.h"

// NAMEHERE Studio Headers
#include "INAMEHEREDoc.h"
#include "LuaSourceBuffer.h"

class QPoint;

class NAMEHEREMainWindow;
class NAMEHEREObjectBrowserDoc;
class NAMEHEREWebDoc;
class NAMEHEREScriptDoc;
class DocTabManager;
class DocDockManager;
class NAMEHEREIDEDoc;

namespace RBX
{
	class Instance;
    class DataModel;
}

typedef QMap< QString,RBX::BaldPtr<INAMEHEREDoc> > tKeyDocMap;
typedef QList< RBX::BaldPtr<NAMEHEREScriptDoc> > tScriptDocs;
typedef QList<boost::shared_ptr<RBX::Instance> > tPlayModifiedScriptList;

class NAMEHEREDocManager : public QObject
{
    Q_OBJECT

public:
	
	static NAMEHEREDocManager& Instance();

	void initialize(NAMEHEREMainWindow& mainWindow);
	void shutDown();

    bool requestCloseAllDocs();
    bool requestCloseAndSaveAllDocs();
    void closeAllDocs();

    void registerOpenedScriptDoc(NAMEHEREScriptDoc& doc, LuaSourceBuffer script);
	void deregisterScriptDoc(NAMEHEREScriptDoc& doc);
    void closeDoc(const LuaSourceBuffer& script);
	NAMEHEREObjectBrowserDoc* getObjectBrowserDoc();

    const tScriptDocs& getOpenScriptDocs() { return m_OpenScriptDocs; }
	NAMEHEREScriptDoc* findOpenScriptDoc(LuaSourceBuffer script);
    
    tPlayModifiedScriptList& getPlayModifiedScriptList() { return m_PlayModifiedScriptList; }

	INAMEHEREDoc* getOrCreateDoc(INAMEHEREDoc::RBXDocType docType);
	NAMEHEREScriptDoc* openDoc(LuaSourceBuffer script);
	
    void removeDoc(INAMEHEREDoc& doc);

	void configureDocWidget(INAMEHEREDoc& doc);
	
	void setCurrentDoc(INAMEHEREDoc* doc);
    INAMEHEREDoc* getCurrentDoc() const       { return m_pCurrentDoc; }
    NAMEHEREIDEDoc* getPlayDoc() const        { return m_pPlayDoc; }
	NAMEHEREWebDoc* getStartPageDoc() const   { return m_pWebDoc; }

    int getDocCount() const                 { return m_KeyDocMap.size(); }

	void updateWindowMenu();
    void setDocTitle(INAMEHEREDoc& doc,const QString& displayName,const QString& fileName,const QIcon& icon = QIcon());

    void detachDoc(INAMEHEREDoc& doc,bool dragging);
    void attachDoc(INAMEHEREDoc& doc, int index = -1);

    void setDockHoverOverPos(const QPoint& globalPos);
    bool attemptAttach(INAMEHEREDoc& doc,const QPoint& globalPos);

    void setPlayDocCentralWidget();
    void restoreCentralWidget();

Q_SIGNALS:
	void currentDocChanged();

public Q_SLOTS:

	void openDoc(shared_ptr<RBX::Instance> script, int lineNumber);
	
    void onActivateWindow();

private:

	NAMEHEREDocManager();
	virtual ~NAMEHEREDocManager();

    void activateDoc(int index);
    RBX::BaldPtr<NAMEHEREMainWindow>          m_pMainWindow;
	tKeyDocMap			                    m_KeyDocMap;
	RBX::BaldPtr<INAMEHEREDoc>                m_pCurrentDoc;
	RBX::BaldPtr<NAMEHEREIDEDoc>              m_pPlayDoc;
	RBX::BaldPtr<NAMEHEREObjectBrowserDoc>    m_pObjectBrowser;
	RBX::BaldPtr<NAMEHEREWebDoc>              m_pWebDoc;
	tScriptDocs				                m_OpenScriptDocs;
    RBX::BaldPtr<DocTabManager>             m_DocTabManager;
    RBX::BaldPtr<DocDockManager>            m_DocDockManager;
    tPlayModifiedScriptList                 m_PlayModifiedScriptList;
};
