package com.namehere.client.test;

import android.util.Log;

import com.namehere.client.NAMEHERESettings;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created on 11/4/15.
 */
public class CurlTestHelper {

    private final String TAG = "InstrumentationTest";

    public void init(){

        // load SO
        System.loadLibrary("namehere");

        // init native settings
        NAMEHERESettings.updateNativeSettings();

        // init native Http
        initHttpJNI();
    }

    public void doCurlRequest(String url){

        String response = doCurlRequestJNI(url);
        Log.v(TAG, "CurlTestHelper.doCurlRequest() response!=null:" + (response != null));
    }

    public String getCookieString(){

        String curlCookiesString = getCookieStringJNI();
        Log.v(TAG, "CurlTestHelper.getCookieString() s:" + curlCookiesString);

        String[] curlCookies = curlCookiesString.split(";");

        // Cookies returned by curl is in the format "#HttpOnly_.namehere.com	TRUE	/	FALSE	0	RBXEventTrackerV2	CreateDate=11/11/2015 1:46:19 PM&rbxid=&browserid=3759303105"
        String cookieString = "";

        Pattern prefix = Pattern.compile("\tTRUE\t/\tFALSE\t\\d*\t(.*)");
        Log.v(TAG, "CurlTestHelper.getCookieString() num:" + curlCookies.length);
        for(String cookie: curlCookies){
            Matcher m = prefix.matcher(cookie);
            if(m.find()) {
                cookie = m.group(1);
                cookie = cookie.replaceFirst("\t", "=");
            }
            cookieString = cookieString + cookie + ";";
        }

        return cookieString;
    }

    private static native void initHttpJNI();
    private static native String doCurlRequestJNI(String url);
    private static native String getCookieStringJNI();
}
