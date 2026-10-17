#include <Windows.h>
#include <stdio.h>
#include <string.h>

#if defined(XULRUNNER_SDK)
#include <npapi.h>
#include "npupp.h"
#include <npruntime.h>
#elif defined(WEBKIT_DARWIN_SDK)
#include <Webkit/npapi.h>
#include <WebKit/npfunctions.h>
#include <WebKit/npruntime.h>
#define OSCALL
#elif defined(WEBKIT_WINMOBILE_SDK) /* WebKit SDK on Windows */
#ifndef PLATFORM
#define PLATFORM(x) defined(x)
#endif
#include <npfunctions.h>
#ifndef OSCALL
#define OSCALL WINAPI
#endif
#endif

#include <vector>
#include "NAMEHEREProxy_i.h"
#include "rolbox_adapter.h"
#include "nppinstancelist.h"
#include <map>
#include <string>

typedef char* String;

// Constant declaration
const String kNAMEHEREHost = "namehere.com";
const String kNAMEHERETestSiteHost = "nameherelabs.com";

//const String kNAMEHEREHost = "LocalHost";

const String kMIMEDescription  = "application/x-vnd-namehere-launcher:.launcher:info@namehere.com";
const String kPluginDescriptionString = "<a href=\"http://www.namehere.com/\">NAMEHERELauncherPlugin</a> plugin.";
const String kStartComOk = "CoInitialize Ok";
const String kStartComFail = "CoInitialize fail";
const String kStopComOk = "CoUninitialize Ok";
const String kStopComFail = "CoUninitialize fail";


NPInstanceList objList;

//static NPObject *so              = NULL;
static NPNetscapeFuncs *npnfuncs = NULL;

//=================================================================================
//=================log functions ==================================================
//=================================================================================
void NPInstanceList::writeLogEntryRaw(char *entry)
{
	EnterCriticalSection(&logLock);
	SYSTEMTIME sysTime = {};
	GetSystemTime(&sysTime);
	char timeBuf[128];
	sprintf(timeBuf, "%02d-%02d-%d %02d:%02d:%02d ", sysTime.wMonth, sysTime.wDay, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	log << timeBuf << entry << "\n";
	log.flush();
	LeaveCriticalSection(&logLock);
}

void NPInstanceList::writeLogEntry(char *format, ...)
{
	if (dontLog) {
		return;
	}
	

	const int stackBufSize = 512;
	char buf[stackBufSize];

	va_list args;
	va_start(args, format);

	int size = _vscprintf(format, args) + 1;
	if (size < stackBufSize)
	{
		vsprintf(buf, format, args);
		writeLogEntryRaw(buf);
	}
	else
	{
		//hopefully we won't go this pass very often
		std::vector<char> bbuf(size);
		vsprintf(&bbuf[0], format, args);
		writeLogEntryRaw(&bbuf[0]);
	}

	va_end(args);
}

bool isNAMEHEREHost(NPP instance,char* hostString) {
	bool result = false;
	NPError err = -1;
    NPObject* sWindowObj;
	NPVariant rval;
	NPVariant vdoc;
	NPObject* doc;

    err = npnfuncs->getvalue(instance, NPNVWindowNPObject, &sWindowObj);

	err = npnfuncs->getproperty(instance, sWindowObj, npnfuncs->getstringidentifier("location"), &vdoc);
	doc = NPVARIANT_TO_OBJECT(vdoc);
	err = npnfuncs->getproperty(instance, doc,npnfuncs->getstringidentifier("host"), &rval);

	
	if (NPVARIANT_IS_STRING(rval)) {
		std::vector<char> zhost(rval.value.stringValue.utf8length + 1);
		memcpy(&zhost[0], rval.value.stringValue.utf8characters, rval.value.stringValue.utf8length);
		zhost[rval.value.stringValue.utf8length] = 0;

		char * siteCopy = _strdup(hostString);

		_strupr(&zhost[0]);
		_strupr(siteCopy);

		// search namehere site
		if (strstr(&zhost[0], siteCopy) != NULL){
			result = true;
			objList.writeLogEntry("location.host contains substring=%s", hostString);
		} else {
			objList.writeLogEntry("location.host DOES NOT contain substring=%s", hostString);
		}

		// release memory
		free(siteCopy);
	} else {// for Debug only
		//sprintf(buff, "rval IS NOT STRING\n");
		//logmsg(buff);
	}

	npnfuncs->releasevariantvalue(&rval);
	npnfuncs->releasevariantvalue(&vdoc);
	//npnfuncs->releaseobject(doc); //duplicate release of vdoc, might have caused problems
	npnfuncs->releaseobject(sWindowObj);

	return result;
}

//=================================================================================
//=================NAMEHERE API implementation=======================================
//=================================================================================


/**
* Invoke HelloWorld method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterHelloWorld(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterHelloWorld 0x%8.8x", (void*)obj);
	ILauncher * namehereLauncher = objList.find(obj);
	
	result->type = NPVariantType_Int32;

	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		result->value.intValue = NAMEHEREAdapterHelloWorld(namehereLauncher);
	}

	objList.writeLogEntry("NPNAMEHEREProxy: NAMEHEREAdapterHelloWorld result = %d", result->value.intValue);

	return true;
}


/**
* Invoke Hello method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterHello(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	char ssMess[2048];
	uint32_t size = (uint32_t)strlen(ssMess) + 1;
	objList.writeLogEntry("invokeNAMEHEREAdapterHello 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->type = NPVariantType_Int32;
		result->value.intValue = -1;
	} else {
		result->type = NPVariantType_String;
		int hr = NAMEHEREAdapterHello(namehereLauncher, ssMess);
		objList.writeLogEntry("INAMEHERELauncher->Hello() result %d", hr);
		size = (uint32_t)strlen(ssMess) + 1;
		result->value.stringValue.utf8length = size - 1;

		result->value.stringValue.utf8characters = (const NPUTF8 *)npnfuncs->memalloc(size);
		strcpy( (char *)result->value.stringValue.utf8characters, ssMess );
	}

	objList.writeLogEntry("NAMEHEREAdapterHello result=%d size=%d ssMess=%s", result->value.intValue, size, ssMess);

	return true;
}

/**
* Invoke StartGame method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterStartGame(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterStartGame 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		if (argCount != 2) {
			result->value.intValue = -1;

			objList.writeLogEntry("NAMEHEREAdapterStartGame argCount!=2");
			return true;
		}
		if (args[0].type != NPVariantType_String || args[1].type != NPVariantType_String ){
			result->value.intValue = -1;

			objList.writeLogEntry("NAMEHEREAdapterStartGame Bad arguments type : MUST BE String");
			return true;
		}

		std::vector<char> zauthFF(args[0].value.stringValue.utf8length + 1);
		memcpy(&zauthFF[0], args[0].value.stringValue.utf8characters, args[0].value.stringValue.utf8length);
		zauthFF[args[0].value.stringValue.utf8length] = 0;

		std::vector<char> zscriptFF(args[1].value.stringValue.utf8length + 1);
		memcpy(&zscriptFF[0], args[1].value.stringValue.utf8characters, args[1].value.stringValue.utf8length);
		zscriptFF[args[1].value.stringValue.utf8length] = 0;

		objList.writeLogEntry("NAMEHEREAdapterStartGame Auth = %s, Script = %s", &zauthFF[0], &zscriptFF[0]);
		result->value.intValue = NAMEHEREAdapterStartGame(namehereLauncher, &zauthFF[0], &zscriptFF[0]);
	}

	objList.writeLogEntry("NAMEHEREAdapterStartGame result=%d", result->value.intValue);
	return true;
}


/**
* Invoke PreStartGame method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterPreStartGame(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterPreStartGame 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		if (argCount != 0) {
			result->value.intValue = -1;

			objList.writeLogEntry("NAMEHEREAdapterPreStartGame argCount!=0");
			return true;
		}

		result->value.intValue = NAMEHEREAdapterPreStartGame(namehereLauncher);
	}

	objList.writeLogEntry("NAMEHEREAdapterPreStartGame result=%d", result->value.intValue);
	return true;
}


/**
* Invoke GetInstallHost method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterGet_InstallHost(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	char ssMess[2048];
	uint32_t size=(uint32_t)strlen(ssMess) + 1;

	objList.writeLogEntry("invokeNAMEHEREAdapterGet_InstallHost 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->type = NPVariantType_Int32;
		result->value.intValue = -1;
	} else {
		result->type = NPVariantType_String;
		int hr = NAMEHEREAdapterGet_InstallHost(namehereLauncher, ssMess);

		size=(uint32_t)strlen(ssMess) + 1;
		// buggy way of doing things, once web site updated use commented line bellow
		result->value.stringValue.utf8length = size;
		// result->value.stringValue.utf8length = size - 1;

		result->value.stringValue.utf8characters = (const NPUTF8 *) npnfuncs->memalloc(size);
		strcpy((char *)result->value.stringValue.utf8characters, ssMess);
	}

	objList.writeLogEntry("NAMEHEREAdapterGet_InstallHost result=%d size=%d ssMess=%s", result->value.intValue, size, ssMess);

	return true;
}

/**
* Invoke GetVersion method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterGet_Version(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	char ssMess[2048];
	uint32_t size=(uint32_t)strlen(ssMess) + 1;

	objList.writeLogEntry("invokeNAMEHEREAdapterGet_Version 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->type = NPVariantType_Int32;
		result->value.intValue = -1;
	} else {
		result->type = NPVariantType_String;
		int hr = NAMEHEREAdapterGet_Version(namehereLauncher, ssMess);

		size=(uint32_t)strlen(ssMess) + 1;
		// buggy way of doing things, once web site updated use commented line bellow
		result->value.stringValue.utf8length = size;
		// result->value.stringValue.utf8length = size - 1;

		result->value.stringValue.utf8characters = (const NPUTF8 *) npnfuncs->memalloc(size);
		strcpy((char *)result->value.stringValue.utf8characters, ssMess);
	}

	objList.writeLogEntry("NAMEHEREAdapterGet_Version result=%d size=%d ssMess=%s", result->value.intValue, size, ssMess);

	return true;
}

/**
* Invoke Update method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterUpdate(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterUpdate 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		result->value.intValue = NAMEHEREAdapterUpdate(namehereLauncher);
	}

	return true;
}


/**
* Invoke PutAuthentificationTicket method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterPut_AuthenticationTicket(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterPut_AuthenticationTicket 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		if (argCount != 1) {
			result->value.intValue = -1;

			objList.writeLogEntry("NAMEHEREAdapterPut_AuthenticationTicket argCount!=1");
			return true;
		}

		if (args[0].type != NPVariantType_String) {
			result->value.intValue = -1;

			objList.writeLogEntry("NAMEHEREAdapterPut_AuthenticationTicket Bad arguments type : MUST BE String");
			return true;
		}
		std::vector<char> zticket(args[0].value.stringValue.utf8length + 1);
		memcpy(&zticket[0], args[0].value.stringValue.utf8characters, args[0].value.stringValue.utf8length);
		zticket[args[0].value.stringValue.utf8length] = 0;

		objList.writeLogEntry("NAMEHEREAdapterPut_AuthenticationTicket Ticket = %s", &zticket[0]);
		result->value.intValue = NAMEHEREAdapterPut_AuthenticationTicket(namehereLauncher, &zticket[0]);
	}

	objList.writeLogEntry("NAMEHEREAdapterPut_AuthenticationTicket result=%d", result->value.intValue);
	
	return true;
}

/**
* Invoke IsGameStarted method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterGet_IsGameStarted(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsGameStarted 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Bool;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.boolValue = 0;
	} else {
		bool started;
		int hr = NAMEHEREAdapterGet_IsGameStarted(namehereLauncher, &started);
		objList.writeLogEntry("NAMEHEREAdapterGet_IsGameStarted HRESULT = %d", hr);
		result->value.boolValue = started;
	}

	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsGameStarted result=%d", result->value.boolValue);

	return true;
}

/**
* Invoke SetSilentModeEnabled method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterGet_SetSilentModeEnabled(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsGameStarted 0x%8.8x", (void*)obj);

	char *cbufTicket = NULL;

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		if (argCount != 1) {
			result->value.intValue = -1;

			objList.writeLogEntry("NAMEHEREAdapterGet_SetSilentModeEnabled argCount!=1");
			return true;
		}

		if (args[0].type != NPVariantType_Bool) {
			result->value.intValue = -1;

			objList.writeLogEntry("NAMEHEREAdapterGet_SetSilentModeEnabled Bad arguments type : MUST be BOOL");
			return true;
		}

		result->value.intValue = NAMEHEREAdapterGet_SetSilentModeEnabled(namehereLauncher, NPVARIANT_TO_BOOLEAN(args[0]));
	}

	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsGameStarted result=%d", result->value.intValue);

	return true;
}

/**
* Invoke UnhideApp method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterGet_UnhideApp(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_UnhideApp 0x%8.8x", (void*)obj);

	char *cbufTicket = NULL;

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		if (argCount != 0) {
			result->value.intValue = -1;

			objList.writeLogEntry("invokeNAMEHEREAdapterGet_UnhideApp argCount!=0");
			return true;
		}

		result->value.intValue = NAMEHEREAdapterGet_UnhideApp(namehereLauncher);
	}

	objList.writeLogEntry("invokeNAMEHEREAdapterGet_UnhideApp result=%d", result->value.intValue);

	return true;
}

/**
* Invoke BringAppToFront method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterBringAppToFront(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterBringAppToFront 0x%8.8x", (void*)obj);

	char *cbufTicket = NULL;

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		if (argCount != 0) {
			result->value.intValue = -1;

			objList.writeLogEntry("invokeNAMEHEREAdapterBringAppToFront argCount!=0");
			return true;
		}

		result->value.intValue = NAMEHEREAdapterBringAppToFront(namehereLauncher);
	}

	objList.writeLogEntry("invokeNAMEHEREAdapterBringAppToFront result=%d", result->value.intValue);

	return true;
}

/**
* Invoke ResetLaunchState method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterResetLaunchState(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterResetLaunchState 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		if (argCount != 0) {
			result->value.intValue = -1;

			objList.writeLogEntry("invokeNAMEHEREAdapterBringAppToFront argCount!=0");
			return true;
		}

		result->value.intValue = NAMEHEREAdapterResetLaunchState(namehereLauncher);
	}

	objList.writeLogEntry("invokeNAMEHEREAdapterBringAppToFront result=%d", result->value.intValue);

	return true;
}

/**
* Invoke SetEditMode method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterSetEditMode(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterSetEditMode 0x%8.8x", (void*)obj);

	char *cbufTicket = NULL;

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		if (argCount != 0) {
			result->value.intValue = -1;

			objList.writeLogEntry("invokeNAMEHEREAdapterSetEditMode argCount!=0");
			return true;
		}

		result->value.intValue = NAMEHEREAdapterSetEditMode(namehereLauncher);
	}

	objList.writeLogEntry("invokeNAMEHEREAdapterSetEditMode result=%d", result->value.intValue);

	return true;
}


/**
* Invoke SetStartInHiddenMode method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterGet_SetStartInHiddenMode(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_SetStartInHiddenMode 0x%8.8x", (void*)obj);

	char *cbufTicket = NULL;

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		if (argCount != 1) {
			result->value.intValue = -1;

			objList.writeLogEntry("NAMEHEREAdapterGet_SetStartInHiddenMode argCount!=1");
			return true;
		}

		if (args[0].type != NPVariantType_Bool) {
			result->value.intValue = -1;

			objList.writeLogEntry("NAMEHEREAdapterGet_SetStartInHiddenMode Bad arguments type : MUST be BOOL");
			return true;
		}

		result->value.intValue = NAMEHEREAdapterGet_SetStartInHiddenMode(namehereLauncher, NPVARIANT_TO_BOOLEAN(args[0]));
	}

	objList.writeLogEntry("invokeNAMEHEREAdapterGet_SetStartInHiddenMode result=%d", result->value.intValue);

	return true;
}

/**
* Invoke get_IsUpToDate method from NAMEHERE ActiveX
*
*/
static bool invokeNAMEHEREAdapterGet_IsUpToDate(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsUpToDate 0x%8.8x", (void*)obj);

	ILauncher *namehereLauncher = objList.find(obj);

	result->type = NPVariantType_Bool;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.boolValue = 0;
	} else {
		bool isUpToDate;
		int hr = NAMEHEREAdapterGet_IsUpToDate(namehereLauncher, &isUpToDate);
		objList.writeLogEntry("NAMEHEREAdapterGet_IsUpToDate HRESULT = %d", hr);
		result->value.boolValue = isUpToDate;
	}

	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsUpToDate result=%d", result->value.boolValue);

	return true;
}

static bool invokeNAMEHEREAdapterGetKeyValue(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGetKeyValue");

	ILauncher *namehereLauncher = objList.find(obj);
	result->type = NPVariantType_String;
	if (namehereLauncher == NULL) {
		result->type = NPVariantType_Int32;
		result->value.intValue = -1;
	} else {
		if (argCount != 1) {
			result->type = NPVariantType_Int32;
			result->value.intValue = -1;

			objList.writeLogEntry("invokeNAMEHEREAdapterGetKeyValue argCount != 1");
			return true;
		}
		
		if (args[0].type != NPVariantType_String) {
			result->value.intValue = -1;
			result->type = NPVariantType_Int32;

			objList.writeLogEntry("invokeNAMEHEREAdapterGetKeyValue arg[0] : must be string");
			return true;
		}
		char *key = (char *)args[0].value.stringValue.utf8characters;

		std::vector<char> zkey(args[0].value.stringValue.utf8length + 1);
		memcpy(&zkey[0], args[0].value.stringValue.utf8characters, args[0].value.stringValue.utf8length);
		zkey[args[0].value.stringValue.utf8length] = 0;
		
		objList.writeLogEntry("invokeNAMEHEREAdapterGetKeyValue key = %s len = %d", &zkey[0], args[0].value.stringValue.utf8length);
		char strValue[2048] = {0};

		int hr = NAMEHEREAdapterGetKeyValue(namehereLauncher, &zkey[0], strValue);
		if (!FAILED(hr))
		{
			int size = (uint32_t)strlen(strValue) + 1;
			result->value.stringValue.utf8length = size - 1;
			objList.writeLogEntry("invokeNAMEHEREAdapterGetKeyValue value = %s len = %d", strValue, result->value.stringValue.utf8length);

			result->value.stringValue.utf8characters = (const NPUTF8 *) npnfuncs->memalloc(size);
			strcpy((char *)result->value.stringValue.utf8characters, strValue);
		} else {
			result->value.intValue = -1;
			result->type = NPVariantType_Int32;

			objList.writeLogEntry("invokeNAMEHEREAdapterGetKeyValue failed error = %d", hr);
		}
	}
	return true;
}

static bool invokeNAMEHEREAdapterSetKeyValue(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterSetKeyValue");

	ILauncher *namehereLauncher = objList.find(obj);
	result->type = NPVariantType_Int32;
	result->value.intValue = -1;
	if (namehereLauncher != NULL) {
		if (argCount != 2) {
			objList.writeLogEntry("invokeNAMEHEREAdapterSetKeyValue argCount != 2");
			return true;
		}

		if (args[0].type != NPVariantType_String) {
			objList.writeLogEntry("invokeNAMEHEREAdapterSetKeyValue arg[0] : must be string");
			return true;
		}

		if (args[1].type != NPVariantType_String) {
			objList.writeLogEntry("invokeNAMEHEREAdapterSetKeyValue arg[1] : must be string");
			return true;
		}

		std::vector<char> zkey(args[0].value.stringValue.utf8length + 1);
		memcpy(&zkey[0], args[0].value.stringValue.utf8characters, args[0].value.stringValue.utf8length);
		zkey[args[0].value.stringValue.utf8length] = 0;

		std::vector<char> zvalue(args[1].value.stringValue.utf8length + 1);
		memcpy(&zvalue[0], args[1].value.stringValue.utf8characters, args[1].value.stringValue.utf8length);
		zvalue[args[1].value.stringValue.utf8length] = 0;

		result->value.intValue = NAMEHEREAdapterSetKeyValue(namehereLauncher, &zkey[0], &zvalue[0]);
		if (result->value.intValue != 0) {
			objList.writeLogEntry("NAMEHEREAdapterSetKeyValue returned %d", result->value.intValue);
		}
	}

	return true;
}

static bool invokeNAMEHEREAdapterDeleteKeyValue(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterDeleteKeyValue");

	ILauncher *namehereLauncher = objList.find(obj);
	result->type = NPVariantType_Int32;
	result->value.intValue = -1;
	if (namehereLauncher != NULL) {
		if (argCount != 1) {
			objList.writeLogEntry("invokeNAMEHEREAdapterDeleteKeyValue argCount != 1");
			return true;
		}

		if (args[0].type != NPVariantType_String) {
			objList.writeLogEntry("invokeNAMEHEREAdapterDeleteKeyValue arg[0] : must be string");
			return true;
		}

		std::vector<char> zkey(args[0].value.stringValue.utf8length + 1);
		memcpy(&zkey[0], args[0].value.stringValue.utf8characters, args[0].value.stringValue.utf8length);
		zkey[args[0].value.stringValue.utf8length] = 0;

		result->value.intValue = NAMEHEREAdapterDeleteKeyValue(namehereLauncher, &zkey[0]);
		if (result->value.intValue != 0) {
			objList.writeLogEntry("NAMEHEREAdapterDeleteKeyValue returned %d", result->value.intValue);
		}
	}
	return true;
}

//=================================================================================
//=================NS API implementation===========================================
//=================================================================================

typedef bool (*invokePtr)(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result);

static std::map<std::string, invokePtr> InitKnownMethods()
{
	std::map<std::string, invokePtr> result;

	// javascript interface method declaration
	result["HelloWorld"] = invokeNAMEHEREAdapterHelloWorld;
	result["Hello"] = invokeNAMEHEREAdapterHello;
	result["StartGame"] = invokeNAMEHEREAdapterStartGame;
	result["Get_InstallHost"] = invokeNAMEHEREAdapterGet_InstallHost;
	result["Get_Version"] = invokeNAMEHEREAdapterGet_Version;
	result["Update"] = invokeNAMEHEREAdapterUpdate;
	result["Put_AuthenticationTicket"] = invokeNAMEHEREAdapterPut_AuthenticationTicket;
	result["Get_GameStarted"] = invokeNAMEHEREAdapterGet_IsGameStarted;
	result["SetSilentModeEnabled"] = invokeNAMEHEREAdapterGet_SetSilentModeEnabled;
	result["SetStartInHiddenMode"] = invokeNAMEHEREAdapterGet_SetStartInHiddenMode;
	result["UnhideApp"] = invokeNAMEHEREAdapterGet_UnhideApp;
	result["DeleteKeyValue"] = invokeNAMEHEREAdapterDeleteKeyValue;
	result["Get_IsUpToDate"] = invokeNAMEHEREAdapterGet_IsUpToDate;
	result["PreStartGame"] = invokeNAMEHEREAdapterPreStartGame;
	result["GetKeyValue"] = invokeNAMEHEREAdapterGetKeyValue;
	result["SetKeyValue"] = invokeNAMEHEREAdapterSetKeyValue;
	result["DeleteKeyValue"] = invokeNAMEHEREAdapterDeleteKeyValue;
	result["BringAppToFront"] = invokeNAMEHEREAdapterBringAppToFront;
	result["ResetLaunchState"] = invokeNAMEHEREAdapterResetLaunchState;
	result["SetEditMode"] = invokeNAMEHEREAdapterSetEditMode;

	return result;
}

/**
* Invoke GetInstallHost method from NAMEHERE ActiveX
*
*/
static bool invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invoke");
	static std::map<std::string, invokePtr> methods = InitKnownMethods();

	char *name = npnfuncs->utf8fromidentifier(methodName);
	if (name != NULL && methods[name] != NULL)
	{
		return methods[name](obj, args, argCount, result);
	} 
	else
	{
		// aim exception handling
		npnfuncs->setexception(obj, "exception during invocation");
		return false;
	}
}

static bool hasProperty(NPObject *obj, NPIdentifier propertyName) {
	objList.writeLogEntry("hasProperty");
	return false;
}

static bool getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
	objList.writeLogEntry("getProperty");
	return false;
}

static bool hasMethod(NPObject* obj, NPIdentifier methodName) {
	objList.writeLogEntry("hasMethod");
	return true;
}

static bool invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeDefault");
	result->type = NPVariantType_Int32;
	result->value.intValue = 42;
	return true;
}

static NPClass npcRefObject = {
	NP_CLASS_STRUCT_VERSION,
	NULL,
	NULL,
	NULL,
	hasMethod,
	invoke,
	invokeDefault,
	hasProperty,
	getProperty,
	NULL,
	NULL,
};

/* NPP */

static NPError nevv(NPMIMEType pluginType, NPP instance, uint16 mode, int16 argc, char *argn[], char *argv[], NPSavedData *saved) {
	objList.writeLogEntry("newv");

	// check if current page URL contains namehere site
	bool result = isNAMEHEREHost(instance, kNAMEHEREHost);
	if (!result)
		result = isNAMEHEREHost(instance, kNAMEHERETestSiteHost);

	if( result == false ){
		objList.writeLogEntry("newv isNAMEHEREHost = FALSE");
		return NPERR_NO_ERROR;
	}

	// create link between NPP instance and ILauncher * INAMEHERELauncher 

	NP_NAMEHERE_INSTANCE * object = (NP_NAMEHERE_INSTANCE *)malloc(sizeof(NP_NAMEHERE_INSTANCE));
	if (object == NULL) {
		objList.writeLogEntry("newv malloc(sizeof(NP_NAMEHERE_INSTANCE) -> NULL");
		return NPERR_OUT_OF_MEMORY_ERROR;
	}

	object->instance = instance;
	// create ILauncher * INAMEHERELauncher
	object->INAMEHERELauncher = GetINAMEHERELauncherInstance();
	if (object->INAMEHERELauncher == NULL) {
		objList.writeLogEntry("newv GetINAMEHERELauncherInstance() = NULL");
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	// create NPObject *so
	object->so = npnfuncs->createobject(instance, &npcRefObject);
	if (object->so == NULL) {
		objList.writeLogEntry("newv npnfuncs->createobject(instance, &npcRefObject) = NULL");
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	// now add this link to NPInstanceList objList;
	objList.add(object);

	return NPERR_NO_ERROR;
}

static NPError destroy(NPP instance, NPSavedData **save) {
	objList.writeLogEntry("destroy");

	NP_NAMEHERE_INSTANCE * object = objList.findAndRemove(instance);
	if (object == NULL ) {
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	// release ILauncher * INAMEHERELauncher
	object->INAMEHERELauncher->Release();
	// release NPObject *so
	npnfuncs->releaseobject(object->so);
	// now release memory
	free(object);

	return NPERR_NO_ERROR;
}

static NPError getValue(NPP instance, NPPVariable variable, void *value) {
	NP_NAMEHERE_INSTANCE * object;
	switch(variable) {
	default:
		objList.writeLogEntry("getvalue - default");
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
		objList.writeLogEntry("getvalue - name string");
		*((char **)value) = "NAMEHERELauncherPlugin";
		break;
	case NPPVpluginDescriptionString:
		objList.writeLogEntry("getvalue - description string");
		*((char **)value) = kPluginDescriptionString;
		break;
		
	case NPPVpluginScriptableNPObject:
		object = objList.find(instance);
		objList.writeLogEntry("getvalue - scriptable object : NPP instance=%p objList.find(instance)=%p", instance, object);
		if( object == NULL ) {
			return NPERR_INVALID_INSTANCE_ERROR;
		}
		npnfuncs->retainobject(object->so);
		*(NPObject **)value = object->so;
		break;
#if defined(XULRUNNER_SDK)
	case NPPVpluginNeedsXEmbed:
		objList.writeLogEntry("getvalue - xembed");
		*((PRBool *)value) = PR_FALSE;
		break;
#endif
	}
	return NPERR_NO_ERROR;
}

/* expected by Safari on Darwin */
static NPError  handleEvent(NPP instance, void *ev) {
	objList.writeLogEntry("handleEvent");
	return NPERR_NO_ERROR;
}

/* expected by Opera */
static NPError setWindow(NPP instance, NPWindow* pNPWindow) {
	objList.writeLogEntry("setWindow");
	return NPERR_NO_ERROR;
}

/* EXPORT */
#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
	objList.writeLogEntry("NP_GetEntryPoints");
	nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
	nppfuncs->newp          = nevv;
	nppfuncs->destroy       = destroy;
	nppfuncs->getvalue      = getValue;
	nppfuncs->event         = handleEvent;
	nppfuncs->setwindow     = setWindow;

	return NPERR_NO_ERROR;
}

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32)(x)) & 0xff00) >> 8)
#endif

NPError OSCALL NP_Initialize(NPNetscapeFuncs *npnf
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
			, NPPluginFuncs *nppfuncs)
#else
			)
#endif
{
	objList.writeLogEntry("NPNAMEHEREProxy: NP_Initialize");
	if (npnf == NULL) {
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	if (HIBYTE(npnf->version) > NP_VERSION_MAJOR) {
		return NPERR_INCOMPATIBLE_VERSION_ERROR;
	}

	npnfuncs = npnf;
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
	NP_GetEntryPoints(nppfuncs);
#endif

	if (!isComStarted()){
		StartCom();
	}
	if (isComStarted()) {
		objList.writeLogEntry(kStartComOk);
	} else {
		objList.writeLogEntry(kStartComFail);
	}
	return NPERR_NO_ERROR;
}

NPError OSCALL NP_Shutdown() {
	objList.writeLogEntry("NPNAMEHEREProxy: NP_Shutdown");

	if (isComStarted()){
		StopCom();
	}

	if (!isComStarted()) {
		objList.writeLogEntry(kStopComOk);
	} else {
		objList.writeLogEntry(kStopComFail);
	}

	return NPERR_NO_ERROR;
}

char * NP_GetMIMEDescription(void) {
	objList.writeLogEntry("NPNAMEHEREProxy: NP_GetMIMEDescription");
	return kMIMEDescription;
}



/* needs to be present for WebKit based browsers */
NPError OSCALL NP_GetValue(void *npp, NPPVariable variable, void *value) {
	return getValue((NPP)npp, variable, value);
}

#ifdef __cplusplus
}
#endif
