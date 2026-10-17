/**
 * NAMEHEREToolBox.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

#include <boost/shared_ptr.hpp>

// NAMEHERE Headers
#include "reflection/Type.h"

// NAMEHERE Studio Headers
#include "NAMEHEREBrowser.h"
#include "NAMEHEREWebPage.h"

namespace RBX
{
	class DataModel;
}

class RbxWorkspace;


class NAMEHEREToolBox : public QWidget
{
	Q_OBJECT
	
public:
	NAMEHEREToolBox();
	void setDataModel(boost::shared_ptr<RBX::DataModel> pDataModel);
	QString getTitleFromUrl(const QString &urlString);
	
public Q_SLOTS:
	void onAuthenticationChanged(bool isAuthenticated);
	void loadUrl(const QString url);

private:
	NAMEHEREBrowser *m_pWebView;
	NAMEHEREWebPage *m_pWebPage;
	boost::shared_ptr<RBX::DataModel>	m_pDataModel;
	
	QString m_urlString;
	
	boost::shared_ptr<RbxWorkspace> m_pWorkspace;
	
	void setupWebView(QWidget *wrapperWidget);
	bool reloadView;
	
private Q_SLOTS:
	void initJavascript();
	
};
