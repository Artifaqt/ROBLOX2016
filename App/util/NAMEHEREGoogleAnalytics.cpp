/**
* NAMEHEREGoogleAnalytics.cpp
* Copyright (c) 2013 NAMEHERE Corp. All rights reserved.
*/
#include "stdafx.h"

#include "Util/NAMEHEREGoogleAnalytics.h"

#include "Util/Guid.h"
#include "Util/Http.h"
#include "Util/Statistics.h"
#include "Util/Object.h"
#include "util/ThreadPool.h"
#include "util/Analytics.h"
#include "v8datamodel/GameBasicSettings.h"

#include "boost/scoped_ptr.hpp"

using namespace RBX;

FASTFLAGVARIABLE(GoogleAnalyticsTrackingEnabled, false)
DYNAMIC_LOGVARIABLE(GoogleAnalyticsTracking, 0)

DYNAMIC_FASTFLAGVARIABLE(NAMEHEREAnalyticsTrackingEnabled, false)
DYNAMIC_FASTSTRINGVARIABLE(NAMEHEREAnalyticsURL, "http://test.public.ecs.namehere.com/")

FASTFLAGVARIABLE(DebugAnalyticsForceLotteryWin, false)
FASTFLAGVARIABLE(SendStudioEventsWithStudioSID, true)

DYNAMIC_FASTFLAGVARIABLE(DebugAnalyticsSendUserId, true)

DYNAMIC_FASTFLAG(UseNewAnalyticsApi)

namespace
{
	static const int kNumberThreadPoolThreads = 16;

	static std::string googleAccountPropertyID;
	static std::string googleClientID;
	static std::string experimentString;
	static std::string namehereSessionKey;
	static bool canUseGA = false, canUserNAMEHEREEvents = false;
	static bool initialized = false;
	static bool atteptedToUseBeforeInit = false;
  	static rbx::signal<void()> analyticTrackedSignalVar;

	static int analyticsUserID = 0, analyticsPlaceID = 0;

	static const char* namehereProductName = NULL;

	static ThreadPool* threadPool;

	static inline std::string googleCollectionParams(const std::string &hitType)
	{
		std::stringstream ss;
		ss
			<< "v=1"
			<< "&tid=" << googleAccountPropertyID
			<< "&cid=" << googleClientID
			<< "&t=" << hitType;

		if (DFFlag::DebugAnalyticsSendUserId && analyticsUserID > 0)
			 ss << "&userId=" << analyticsUserID;

		return ss.str();
	}

	static inline void httpHandler(std::string url, std::string params, bool isGet)
	{
		FASTLOGS(DFLog::GoogleAnalyticsTracking, "Sending Google Analytics params: %s", params.c_str());

		std::istringstream sparams(params);
		std::string response;

		std::string fullUrl = isGet ? format("%s?%s", url.c_str(), params.c_str()) : url;

		RBX::Http http(fullUrl);
        http.recordStatistics = false;
		http.doNotUseCachedResponse = true;
		try
		{
			if(isGet)
				http.get(response);
			else
				http.post(sparams, Http::kContentTypeDefaultUnspecified, true, response, true);
		}
		catch (const RBX::base_exception &ex)
		{
			FASTLOGS(DFLog::GoogleAnalyticsTracking, "Exception in httpHandler: %s", ex.what());
		}
	}

	static inline bool checkUsage()
	{
		if (!initialized)
		{
			return false;
		}

		RBXASSERT(threadPool);
		RBXASSERT(googleAccountPropertyID.length() > 0);
		RBXASSERT(googleClientID.length() > 0);
		return true;
	}

	static inline void postOrGet(const std::string& url, const std::stringstream& params, bool sync, bool isGet)
	{
        if (sync)
        {
            httpHandler(url, params.str(), isGet);
        }
        else
        {
            bool didSchedule = threadPool->schedule(boost::bind(&httpHandler, url, params.str(), isGet));
            RBXASSERT(didSchedule);
            if (!didSchedule)
            {
                FASTLOGS(DFLog::GoogleAnalyticsTracking, "Google Analytics task was not scheduled in the threadpool: %s", params.str().c_str());
            }
        }
	}
}

namespace RBX
{
namespace NAMEHEREGoogleAnalytics
{
	bool isInitialized()
	{
		return initialized;
	}

    void lotteryInit(const std::string &accountPropertyID, size_t maxThreadScheduleSize, int lotteryThreshold,
		const char * productName /* = NULL */,
		int namehereAnalyticsLottery /* = -1 */,
		const std::string &sessionKey /* = "sessionID=" */)
    {
		if (DFFlag::UseNewAnalyticsApi)
		{
			RBX::Analytics::GoogleAnalytics::lotteryInit(accountPropertyID, lotteryThreshold, productName ? productName : "", namehereAnalyticsLottery, sessionKey);
			return;
		}

		namehereSessionKey = sessionKey;

		NAMEHEREGoogleAnalytics::init(accountPropertyID, maxThreadScheduleSize, productName);

		if(namehereAnalyticsLottery == -1)
		{
			namehereAnalyticsLottery = lotteryThreshold;
		}

        std::size_t lottery = boost::hash_value(googleClientID) % 100;
        FASTLOG1(DFLog::GoogleAnalyticsTracking, "Google analytics lottery number = %d", lottery);
        // initialize google analytics
        if (lottery < (std::size_t)lotteryThreshold || FFlag::DebugAnalyticsForceLotteryWin)
        {
            canUseGA = true;
        }
		if (lottery < (std::size_t)namehereAnalyticsLottery || FFlag::DebugAnalyticsForceLotteryWin)
		{
			canUserNAMEHEREEvents = true;
		}
    }

	std::string generateClientID()
	{
		RBX::Guid guid;
		std::string clientGUID;
		guid.generateStandardGUID(clientGUID);

		// OS Generated GUIDs come in braced form ie "{f81d4fae-7dec-11d0-a765-00a0c91e6bf6}"
		// whereas RFC 4122 http://www.ietf.org/rfc/rfc4122.txt is non-braced.
		clientGUID.erase(std::find(clientGUID.begin(), clientGUID.end(), '{'));
		clientGUID.erase(std::find(clientGUID.begin(), clientGUID.end(), '}'));

		return clientGUID;
	}

	void init(const std::string &accountPropertyID, size_t maxThreadScheduleSize, const char* productName /* = NULL */)
	{
		if (DFFlag::UseNewAnalyticsApi)
		{
			RBX::Analytics::GoogleAnalytics::init(accountPropertyID, productName ? productName : "");
			return;
		}

		if(atteptedToUseBeforeInit)
		{
			RBXASSERT(false); // Did you try to send events before analytics is initialized? Set breakpoints to trackEvent* to find out.
		}

		if (initialized)
		{
			FASTLOGS(DFLog::GoogleAnalyticsTracking, "%s", "Google analytics already initialized!");
			return;
		}

		namehereProductName = productName;

		googleAccountPropertyID = accountPropertyID;
		RBXASSERT(googleAccountPropertyID.length() > 0);

		std::string clientId = GameBasicSettings::singleton().getGoogleAnalyticsClientId();
		if (clientId.length() == 0)
		{
			// generate and store new client ID
			clientId = generateClientID();
			GameBasicSettings::singleton().setGoogleAnalyticsClientId(clientId);
		}

		googleClientID = clientId;
		RBXASSERT(googleClientID.length() > 0);

		static boost::scoped_ptr<ThreadPool> tp(new ThreadPool(kNumberThreadPoolThreads, BaseThreadPool::NoAction, maxThreadScheduleSize));
		threadPool = tp.get();

		initialized = true;
	}

	void setUserID(int id)
	{
		analyticsUserID = id;
	}

	void setPlaceID(int id)
	{
		analyticsPlaceID = id;
	}

	const std::string& getSessionId()
	{
		if (DFFlag::UseNewAnalyticsApi)
			return RBX::Analytics::GoogleAnalytics::getSessionId();
		return googleClientID;
	}

	bool getCanUseAnalytics()
	{
		if (DFFlag::UseNewAnalyticsApi)
			return RBX::Analytics::GoogleAnalytics::getCanUse();
		else
			return canUseGA || canUserNAMEHEREEvents;
	}

	void setCanUseAnalytics()
	{
		if (DFFlag::UseNewAnalyticsApi)
			RBX::Analytics::GoogleAnalytics::setCanUse();
		else
		{
			canUseGA = true;
			canUserNAMEHEREEvents = true;
		}
	}

	void sendEventGA(const char* category,
		const char* action,
		const char* label,
		int value,
		bool sync)
	{
		std::stringstream params;
		params
			<< googleCollectionParams("event")
			<< "&ec=" << category
			<< "&ea=" << action
			<< "&ev=" << value
			<< "&el=" << label;

		postOrGet(kGoogleAnalyticsBaseURL, params, sync, false);
	}

	void sendEventNAMEHERE(const char* category,
		const char* action,
		const char* label,
		int value,
		bool sync)
    {
		if (DFFlag::UseNewAnalyticsApi)
		{
			RBX::Analytics::GoogleAnalytics::sendEventNAMEHERE(category, action, label, value, sync);
			return;
		}

		if(DFFlag::NAMEHEREAnalyticsTrackingEnabled && namehereProductName)
		{
			std::stringstream params;
			params
				<< (FFlag::SendStudioEventsWithStudioSID ? namehereSessionKey : "sessionID=") << googleClientID
				<< "&userID=" << analyticsUserID
				<< "&placeID=" << analyticsPlaceID
				<< "&category=" << category
				<< "&evt=" << action
				<< "&label=" << label
				<< "&value=" << value
				<< experimentString;

			postOrGet(RBX::format("%s%s/e.png",DFString::NAMEHEREAnalyticsURL.c_str(), namehereProductName), params, sync, true);
		}
	}

	void trackEvent(const char *category,
		const char *action,
		const char *label,
		int value,
		bool sync)
	{
		if (DFFlag::UseNewAnalyticsApi)
		{
			RBX::Analytics::GoogleAnalytics::trackEvent(category, action, label, value, sync);
			return;
		}

		if (!initialized)
		{
			atteptedToUseBeforeInit = true;
			return;
		}

		if(canUseGA)
		{
			sendEventGA(category, action, label, value, sync);
		}

		if(canUserNAMEHEREEvents)
		{
			sendEventNAMEHERE(category, action, label, value, sync);
		}

		analyticTrackedSignalVar();
	}

	void trackEventWithoutThrottling(const char* category,
                                     const char* action,
									 const char* label,
									 int value,
									 bool sync)
	{
		if (DFFlag::UseNewAnalyticsApi)
		{
			RBX::Analytics::GoogleAnalytics::trackEventWithoutThrottling(category, action, label, value, sync);
			return;
		}

		if (!initialized)
		{
			atteptedToUseBeforeInit = true;
			return;
		}

		sendEventGA(category, action, label, value, sync);
		sendEventNAMEHERE(category, action, label, value, sync);

        analyticTrackedSignalVar();
	}

	void trackUserTiming(const char *category,
                         const char *variable,
                         int milliseconds,
                         const char *label,
                         bool sync)
	{
		if (DFFlag::UseNewAnalyticsApi)
		{
			RBX::Analytics::GoogleAnalytics::trackUserTiming(category, variable, milliseconds, label, sync);
			return;
		}

		if (!initialized || !canUseGA)
		{
			return;
		}

		std::stringstream params;
		params
			<< googleCollectionParams("timing")
			<< "&utc=" << category
			<< "&utv=" << variable
			<< "&utt=" << milliseconds
			<< "&utl=" << label;

		postOrGet(kGoogleAnalyticsBaseURL, params, sync, false);

        analyticTrackedSignalVar();
	}
    
    rbx::signal<void()>& analyticTrackedSignal()
    {
        return analyticTrackedSignalVar;
    }

	void setExperimentVariation(const std::string& name, int value)
	{
		std::stringstream s;
		s << "&experiment=" << name << "&variation=" << value;
		experimentString = s.str();
	}
    
} // NAMEHEREGoogleAnalytics
} // RBX
