/**
 * NAMEHEREUser.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "NAMEHEREUser.h"

// NAMEHERE headers
#include "Rbx/debug.h"

// NAMEHERE Studio headers
#include "NAMEHERECookieJar.h"
#include "NAMEHERENetworkAccessManager.h"
#include "NAMEHERENetworkReply.h"
#include "NAMEHERESettings.h"
#include "AuthenticationHelper.h"

#include "Util/NAMEHEREGoogleAnalytics.h"
#include "V8DataModel/Stats.h"

NAMEHEREUser& NAMEHEREUser::singleton()
{
    static NAMEHEREUser user;
	return user;
}

NAMEHEREUser::NAMEHEREUser()
	: m_webKitUserId(-1)
{
    connect(&AuthenticationHelper::Instance(), SIGNAL(authenticationChanged(bool)), this, SLOT(onAuthenticationChanged(bool)));
}

NAMEHEREUser::~NAMEHEREUser()
{
	m_webKitUserIDQuery.get();
}

void NAMEHEREUser::init()
{
	if (m_webKitUserId == -1)
		getWebkitUserId();
}

// Keep this user up to sync on auth changes
void NAMEHEREUser::onAuthenticationChanged(bool)
{
	m_webKitUserId = -1;
	getWebkitUserId();
}

void NAMEHEREUser::currentUserReplied(RBX::HttpFuture future)
{
	try {
		QString webKitUserIDStr(QString::fromStdString(future.get()));
		m_webKitUserId = webKitUserIDStr.toInt();
		RBX::Analytics::setUserId(m_webKitUserId);

		// TODO: remove
		RBX::NAMEHEREGoogleAnalytics::setUserID(m_webKitUserId);
	}
	catch(std::exception&)
	{
		m_webKitUserId = 0;
	}
}

void NAMEHEREUser::getWebkitUserId()
{
	std::string url(AuthenticationHelper::getLoggedInUserUrl().toStdString());
    m_webKitUserIDQuery = RBX::HttpAsync::get(url).then(boost::bind(&NAMEHEREUser::currentUserReplied, this, _1));
}

int NAMEHEREUser::getUserId()
{
	if(m_webKitUserId == -1)
	{
		// make sure we are authenticated before querying user id		
		AuthenticationHelper::Instance().waitForHttpAuthentication();
		if (!m_webKitUserIDQuery.valid())
			getWebkitUserId();		

		m_webKitUserIDQuery.get();
	}

	return m_webKitUserId;
}
