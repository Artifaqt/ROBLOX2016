/**
 * NAMEHEREWebPage.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "NAMEHEREWebPage.h"

// Qt Headers
#include <QDesktopServices>
#include <QNetworkReply>
#include <QWebFrame>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QWebHitTestResult>

// NAMEHERE Headers
#include "AuthenticationHelper.h"
#include "FastLog.h"
#include "NAMEHEREBrowser.h"
#include "NAMEHERENetworkAccessManager.h"
#include "NAMEHERESettings.h"
#include "QtUtilities.h"
#include "Util/NAMEHEREGoogleAnalytics.h"

#include <QWebElement>

QString NAMEHEREWebPage::userAgentForUrl(const QUrl &url) const 
{	
	return NAMEHERENetworkAccessManager::Instance().getUserAgent();
}

NAMEHEREWebPage::NAMEHEREWebPage(QWidget* parent) 
    : QWebPage(parent)
{
	setNetworkAccessManager(&NAMEHERENetworkAccessManager::Instance());
	connect(&NAMEHERENetworkAccessManager::Instance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(handleFinished(QNetworkReply*)));
}

void NAMEHEREWebPage::handleFinished(QNetworkReply* reply)
{
	QString status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
	if (status.toInt() != 200 || !reply->hasRawHeader("refresh"))
		return;

	QString acceptHeader = reply->request().rawHeader("accept").data();
	if (!acceptHeader.startsWith("application"))
		return;

	QString url = QString(reply->rawHeader("refresh").data());
	this->mainFrame()->load(url.mid(url.indexOf("=")+1));
}

bool NAMEHEREWebPage::acceptNavigationRequest ( QWebFrame * frame, const QNetworkRequest & request, NavigationType type )
{
    RBX::NAMEHEREGoogleAnalytics::trackEvent("Web", "URLRequest", request.url().toString().toStdString().c_str());

	if (!frame) // This is a new window request, open in default browser
	{
		QDesktopServices::openUrl(request.url());
		return true;
	}
	else
		return QWebPage::acceptNavigationRequest(frame, request, type);
}

QString NAMEHEREWebPage::getDefaultUserAgent() const
{
	return QWebPage::userAgentForUrl(QUrl(""));
}

// If we have an override for the file upload element on this page, use that instead 
// This allows us to inject files into file input tags <input type="file" /> on the page
QString NAMEHEREWebPage::chooseFile( QWebFrame *originatingFrame, const QString& oldFile )
{
	if (m_overideUploadFile.isEmpty())
		return QWebPage::chooseFile(originatingFrame, oldFile);
	else
		return m_overideUploadFile;
}

void NAMEHEREWebPage::setUploadFile(QString selector, QString fileName)
{
	// Store for uploading
	m_overideUploadFile = fileName; 
	
	// Invoke an "file selection" behind the scenes.  This will automatically put the file into
	// the pages file input tag.
	QWebElement button = this->mainFrame()->findFirstElement(selector);
	button.setFocus();
	QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
	QCoreApplication::postEvent (this->view(), event);
}

void NAMEHEREWebPage::triggerAction(QWebPage::WebAction action, bool checked)
{
	if ((action == QWebPage::OpenLinkInNewWindow) && mainFrame() && !m_contextPos.isNull())
	{
		QWebHitTestResult hitTestResult = mainFrame()->hitTestContent(m_contextPos);
		if (!hitTestResult.isNull())
		{
			QUrl url = hitTestResult.linkUrl();
			if (url.isValid() && !url.host().isEmpty())
				QDesktopServices::openUrl(url);
		}
		m_contextPos = QPoint();
	}
	else
	{
		QWebPage::triggerAction(action, checked);
	}
}

bool NAMEHEREWebPage::event(QEvent *evt)
{
	if (evt->type() == QEvent::ContextMenu)
       m_contextPos = static_cast<QContextMenuEvent*>(evt)->pos();

	return QWebPage::event(evt);
}