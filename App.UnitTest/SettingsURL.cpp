#include <boost/test/unit_test.hpp>

#include "NAMEHEREServicesTools.h"

BOOST_AUTO_TEST_SUITE(SettingsUrl)

BOOST_AUTO_TEST_CASE(SettingsUrlProd)
{
	std::string testUrl = "http://clientsettings.api.namehere.com/Setting/QuietGet/Test/?apiKey=TestKey";
	std::string url = GetSettingsUrl("www.namehere.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);

	url = GetSettingsUrl("http://www.namehere.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);
}

BOOST_AUTO_TEST_CASE(SettingsUrlNewGametest)
{
	std::string testUrl = "http://clientsettings.api.gametest1.nameherelabs.com/Setting/QuietGet/Test/?apiKey=TestKey";
	std::string url = GetSettingsUrl("www.gametest1.nameherelabs.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);

	url = GetSettingsUrl("http://www.gametest1.nameherelabs.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);
}

BOOST_AUTO_TEST_CASE(SettingsUrlGametest)
{
	std::string testUrl = "http://clientsettings.api.gametest1.namehere.com/Setting/QuietGet/Test/?apiKey=TestKey";
	std::string url = GetSettingsUrl("www.gametest1.namehere.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);

	url = GetSettingsUrl("http://www.gametest1.namehere.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);
}

BOOST_AUTO_TEST_SUITE_END()
