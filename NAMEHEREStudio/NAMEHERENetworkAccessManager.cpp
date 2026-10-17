/**
 * NAMEHERENetworkAccessManager.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "NAMEHERENetworkAccessManager.h"

// NAMEHERE Headers
#include "QtUtilities.h"
#include "NAMEHERECookieJar.h"
#include "NAMEHEREWebPage.h"
#include "NAMEHERESettings.h"
#include "NAMEHERENetworkReply.h"

NAMEHERENetworkAccessManager::NAMEHERENetworkAccessManager()
: QNetworkAccessManager()
{
	// When we enable the Cookie Persistence, logging into mail.namehere.com stops working, but works fine for www.namehere.com with persistence
	setCookieJar(new NAMEHERECookieJar());
}
void NAMEHERENetworkAccessManager::initUserAgent()
{
	// Set up our useragent
	NAMEHEREWebPage dummy;
	userAgent = dummy.getDefaultUserAgent();
	
	// Only on Mac webkit does not provide the OSX version, add it, will do only for Mac
	QString macOSXVersion = QtUtilities::getMacOSXVersion();
	userAgent = userAgent.replace("Mac OS X", macOSXVersion);
}
QString NAMEHERENetworkAccessManager::getUserAgent()
{
	if (userAgent.isEmpty())
		initUserAgent();
	return userAgent;
}

NAMEHERENetworkReply* NAMEHERENetworkAccessManager::get(const QNetworkRequest& request, bool followRedirection /*= true*/ )
{
	QNetworkReply* reply = QNetworkAccessManager::get(request);
	NAMEHERENetworkReply* nameherereply = new NAMEHERENetworkReply(reply, followRedirection);
	return nameherereply;
}

NAMEHERECookieJar* NAMEHERENetworkAccessManager::cookieJar() const
{
	return dynamic_cast<NAMEHERECookieJar*>(QNetworkAccessManager::cookieJar());
}
