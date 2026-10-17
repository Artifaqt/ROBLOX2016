/**
 * NAMEHEREWebDoc.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QObject>

#include <boost/shared_ptr.hpp>

// NAMEHERE Studio Headers
#include "NAMEHEREBasicDoc.h"

class QComboBox;
class QNetworkReply;
class QUrl;
class QToolBar;
class QSslError;

class NAMEHEREBrowser;
class RbxWorkspace;

class NAMEHEREWebDoc: public QObject, public NAMEHEREBasicDoc
{
	Q_OBJECT
public:

	NAMEHEREWebDoc(const QString& displayName, const QString& keyName);
	~NAMEHEREWebDoc();
	
	bool open(NAMEHEREMainWindow *pMainWindow, const QString &fileName);
	
	INAMEHEREDoc::RBXCloseRequest requestClose() { return INAMEHEREDoc::NO_SAVE_NEEDED; }

	INAMEHEREDoc::RBXDocType docType() { return INAMEHEREDoc::BROWSER; }

	QString fileName() const        { return ""; }
	QString displayName() const     { return m_displayName; }
	QString keyName() const         { return m_keyName; }
			
	bool save(){ return false; }
	bool saveAs(const QString &){ return false; }

	QString saveFileFilters() { return ""; }
	
	QWidget* getViewer();
	
	bool isModified(){ return false; }
	
	void activate();
	void deActivate() { m_bActive = false; }
		
	bool actionState(const QString &actionID, bool &enableState, bool &checkedState);

	bool handlePluginAction(void *, void *) { return false; }

	void handleScriptCommand(const QString &) { return; }	

	bool supportsZeroPlaneGrid()        { return false; }

	boost::shared_ptr<RbxWorkspace> getWorkspace() { return m_pWorkspace; }

public Q_SLOTS:
	void refreshPage();
	
private Q_SLOTS:
	void updateAddressBar(const QUrl&);
	void sslErrorHandler(QNetworkReply* qnr, const QList<QSslError> & errlist);
	void initJavascript();
	void handleHomeClicked();
	void navigateUrl(const QString& url);
	void onAuthenticationChanged(bool);
	void updateTitle(QString title);

private:
	virtual bool doClose();
	void setupWebView(QWidget*);
	QToolBar* setupAddressToolBar(QWidget*);
	

	NAMEHEREBrowser                       *m_pWebView;
	QWidget                             *m_pWrapperWidget;
	boost::shared_ptr<RbxWorkspace>      m_pWorkspace;
	QComboBox                           *m_pAddrInputComboBox;
	QString                              m_displayName; //to be used if file name is empty
	QString                              m_currentUrl;
	QString                              m_homeUrl;
	QString                              m_keyName;
};

