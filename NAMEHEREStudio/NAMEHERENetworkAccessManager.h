/**
 * NAMEHERENetworkAccessManager.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QNetworkAccessManager>

class NAMEHERENetworkReply;
class NAMEHERECookieJar;

class NAMEHERENetworkAccessManager : public QNetworkAccessManager
{
public:
	static NAMEHERENetworkAccessManager& Instance()
	{
		// TODO: NAMEHEREMainWindow should be the parent of this, but it's not a singleton or QObject
		// We instead set the parent during initialization.  Would be better to pass into the constructor
		static NAMEHERENetworkAccessManager singleInstance;
		return singleInstance;
	}

	QString getUserAgent();
	NAMEHERENetworkReply* get(const QNetworkRequest& request, bool followRedirection = true);
	NAMEHERECookieJar* cookieJar() const;

private:
	NAMEHERENetworkAccessManager();
	void initUserAgent();

	QString userAgent;
};