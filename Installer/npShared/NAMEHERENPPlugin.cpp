/*
 *  NAMEHERENPPlugin.cpp
 *  NPNAMEHEREMac
 *
 *  Created by Aaron Wilkowski on 1/4/12.
 *  Copyright 2012 NAMEHERE. All rights reserved.
 *
 */

#include <string>
#include "NAMEHERENPPlugin.h"
#include <map>





NAMEHERENPPlugin *NAMEHERENPPlugin::_instance = 0;

NAMEHERENPPlugin *NAMEHERENPPlugin::Instance(){
	if (_instance == 0) {
		_instance = new NAMEHERENPPlugin;
	}
	return _instance;
}

NAMEHERENPPlugin objList;
NAMEHERENPPlugin *sObjList = objList.Instance();

NPError aCreate(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved){
	return objList.create(pluginType, instance, mode, argc, argn, argv, saved);
}
NPError aDestroy(NPP instance, NPSavedData **save){
	return objList.destroy(instance, save);
}
NPError aGetValue(NPP instance, NPPVariable variable, void *value){
	return objList.getValue(instance, variable, value);
}
NPError aHandleEvent(NPP instance, void *ev){
	return objList.handleEvent(instance, ev);
}
NPError aSetWindow(NPP instance, NPWindow* pNPWindow){
	return objList.setWindow(instance, pNPWindow);
}

extern "C"
{
	//exposed functions
	
	NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) 
	{
		objList.writeLogEntry("NP_GetEntryPoints --> Begin");
		
		if (nppfuncs == NULL)
		{
			objList.writeLogEntry("-- ERROR: nppfuncs is NULL");
			return NPERR_GENERIC_ERROR;
		}
		
		nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
		nppfuncs->newp          = aCreate;
		nppfuncs->destroy       = aDestroy;
		nppfuncs->getvalue      = aGetValue;
		nppfuncs->event         = aHandleEvent;
		nppfuncs->setwindow     = aSetWindow;
		
		objList.writeLogEntry("NP_GetEntryPoints <-- End");
		
		return NPERR_NO_ERROR;
	}
	
#ifndef HIBYTE
#define HIBYTE(x) ((((uint32)(x)) & 0xff00) >> 8)
#endif
	
	NPError OSCALL NP_Initialize(NPNetscapeFuncs *npnf)
	{
		objList.writeLogEntry("NP_Initialize --> Begin");
		
		if(npnf == NULL)
		{
			objList.writeLogEntry("-- Error: npnf is NULL.");
			return NPERR_INVALID_FUNCTABLE_ERROR;
		}
		
		if (HIBYTE(npnf->version) > NP_VERSION_MAJOR)
		{
			objList.writeLogEntry("-- Error: version issue.");
			return NPERR_INCOMPATIBLE_VERSION_ERROR;
		}
		
		//s_netscape_funcs = npnf;
		sObjList->npnfs = npnf; 
		
		objList.writeLogEntry("NP_Initialize <-- End");
		
		return NPERR_NO_ERROR;
	}
	
	NPError OSCALL NP_Shutdown() 
	{
		objList.writeLogEntry("NP_Shutdown --> Begin");
		objList.writeLogEntry("NP_Shutdown <-- End\n");
		return NPERR_NO_ERROR;
	}
	
	const char *NP_GetMIMEDescription(void) 
	{
		return (const char *)MIME_DESCRIPTION;
	}
	
	
	/* needs to be present for WebKit based browsers */
	NPError OSCALL NP_GetValue(void *npp, NPPVariable variable, void *value) 
	{
		objList.writeLogEntry("NP_GetValue --> Begin");
		NPError ret_val = aGetValue((NPP)npp, variable, value);
		objList.writeLogEntry("NP_GetValue <-- End");
		return ret_val;
	}
}
//end C

static bool invokeNAMEHEREAdapterHelloWorld(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterHelloWorld 0x%8.8x", (void*)obj);
	INAMEHERELauncher * namehereLauncher = sObjList->find(obj);
	
	result->type = NPVariantType_Int32;
	
	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		result->value.intValue = namehereLauncher->HelloWorld();
	}
	
	objList.writeLogEntry("NPNAMEHEREProxy: NAMEHEREAdapterHelloWorld result = %d", result->value.intValue);
	
	return true;
}

static bool invokeNAMEHEREAdapterHello(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	char ssMess[2048];
	uint32_t size = (uint32_t)strlen(ssMess) + 1;
	objList.writeLogEntry("invokeNAMEHEREAdapterHello 0x%8.8x", (void*)obj);
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->type = NPVariantType_Int32;
		result->value.intValue = -1;
	} else {
		result->type = NPVariantType_String;
		int hr = namehereLauncher->Hello(ssMess);
		objList.writeLogEntry("INAMEHERELauncher->Hello() result %d", hr);
		size = (uint32_t)strlen(ssMess) + 1;
		result->value.stringValue.UTF8_LENGTH = size - 1;
		
		result->value.stringValue.UTF8_CHARACTERS = (const NPUTF8 *)sObjList->npnfs->memalloc(size);
		strcpy( (char *)result->value.stringValue.UTF8_CHARACTERS, ssMess );
	}
	
	objList.writeLogEntry("NAMEHEREAdapterHello result=%d size=%d ssMess=%s", result->value.intValue, size, ssMess);
	
	return true;
}

static bool invokeNAMEHEREAdapterStartGame(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterStartGame 0x%8.8x", (void*)obj);
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
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
		
		std::vector<char> zauthFF(args[0].value.stringValue.UTF8_LENGTH + 1);
		memcpy(&zauthFF[0], args[0].value.stringValue.UTF8_CHARACTERS, args[0].value.stringValue.UTF8_LENGTH);
		zauthFF[args[0].value.stringValue.UTF8_LENGTH] = 0;
		
		std::vector<char> zscriptFF(args[1].value.stringValue.UTF8_LENGTH + 1);
		memcpy(&zscriptFF[0], args[1].value.stringValue.UTF8_CHARACTERS, args[1].value.stringValue.UTF8_LENGTH);
		zscriptFF[args[1].value.stringValue.UTF8_LENGTH] = 0;
		
		objList.writeLogEntry("NAMEHEREAdapterStartGame Auth = %s, Script = %s", &zauthFF[0], &zscriptFF[0]);
		result->value.intValue = namehereLauncher->StartGame(&zauthFF[0], &zscriptFF[0]);
	}
	
	objList.writeLogEntry("NAMEHEREAdapterStartGame result=%d", result->value.intValue);
	return true;
}

static bool invokeNAMEHEREAdapterPreStartGame(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterPreStartGame 0x%8.8x", (void*)obj);
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
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
		
		result->value.intValue = namehereLauncher->PreStartGame();
	}
	
	objList.writeLogEntry("NAMEHEREAdapterPreStartGame result=%d", result->value.intValue);
	return true;
}


static bool invokeNAMEHEREAdapterGet_InstallHost(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	char ssMess[2048];
	uint32_t size=(uint32_t)strlen(ssMess) + 1;
	
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_InstallHost 0x%8.8x", (void*)obj);
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->type = NPVariantType_Int32;
		result->value.intValue = -1;
	} else {
		result->type = NPVariantType_String;
		int hr = namehereLauncher->GetInstallHost(ssMess);
		
		size=(uint32_t)strlen(ssMess) + 1;
		// buggy way of doing things, once web site updated use commented line bellow
		result->value.stringValue.UTF8_LENGTH = size;
		// result->value.stringValue.UTF8_LENGTH = size - 1;
		
		result->value.stringValue.UTF8_CHARACTERS = (const NPUTF8 *) sObjList->npnfs->memalloc(size);
		strcpy((char *)result->value.stringValue.UTF8_CHARACTERS, ssMess);
	}
	
	objList.writeLogEntry("NAMEHEREAdapterGet_InstallHost result=%d size=%d ssMess=%s", result->value.intValue, size, ssMess);
	
	return true;
}


static bool invokeNAMEHEREAdapterUpdate(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterUpdate 0x%8.8x", (void*)obj);
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
	result->type = NPVariantType_Int32;
	if (namehereLauncher == NULL) {
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.intValue = -1;
	} else {
		result->value.intValue = namehereLauncher->Update();
	}
	
	return true;
}


static bool invokeNAMEHEREAdapterPut_AuthenticationTicket(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterPut_AuthenticationTicket 0x%8.8x", (void*)obj);
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
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
		std::vector<char> zticket(args[0].value.stringValue.UTF8_LENGTH + 1);
		memcpy(&zticket[0], args[0].value.stringValue.UTF8_CHARACTERS, args[0].value.stringValue.UTF8_LENGTH);
		zticket[args[0].value.stringValue.UTF8_LENGTH] = 0;
		
		objList.writeLogEntry("NAMEHEREAdapterPut_AuthenticationTicket Ticket = %s", &zticket[0]);
		result->value.intValue = namehereLauncher->Put_AuthenticationTicket(&zticket[0]);
	}
	
	objList.writeLogEntry("NAMEHEREAdapterPut_AuthenticationTicket result=%d", result->value.intValue);
	
	return true;
}


static bool invokeNAMEHEREAdapterGet_IsGameStarted(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsGameStarted 0x%8.8x", (void*)obj);
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
	result->type = NPVariantType_Bool;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.boolValue = 0;
	} else {
		bool started;
		int hr = namehereLauncher->IsGameStarted(&started);
		objList.writeLogEntry("NAMEHEREAdapterGet_IsGameStarted HRESULT = %d", hr);
		result->value.boolValue = started;
	}
	
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsGameStarted result=%d", result->value.boolValue);
	
	return true;
}

static bool invokeNAMEHEREAdapterGet_SetSilentModeEnabled(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsGameStarted 0x%8.8x", (void*)obj);
	
	char *cbufTicket = NULL;
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
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
		
		result->value.intValue = namehereLauncher->SetSilentModeEnabled(NPVARIANT_TO_BOOLEAN(args[0]));
	}
	
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsGameStarted result=%d", result->value.intValue);
	
	return true;
}

static bool invokeNAMEHEREAdapterGet_SetStartInHiddenMode(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_SetStartInHiddenMode 0x%8.8x", (void*)obj);
	
	char *cbufTicket = NULL;
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
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
		
		result->value.intValue = namehereLauncher->SetStartInHiddenMode(NPVARIANT_TO_BOOLEAN(args[0]));
	}
	
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_SetStartInHiddenMode result=%d", result->value.intValue);
	
	return true;
}


static bool invokeNAMEHEREAdapterGet_UnhideApp(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_UnhideApp 0x%8.8x", (void*)obj);
	
	char *cbufTicket = NULL;
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
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
		
		result->value.intValue = namehereLauncher->UnhideApp();
	}
	
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_UnhideApp result=%d", result->value.intValue);
	
	return true;
}

static bool invokeNAMEHEREAdapterBringAppToFront(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterBringAppToFront 0x%8.8x", (void*)obj);
	
	char *cbufTicket = NULL;
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
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
		
		result->value.intValue = namehereLauncher->BringAppToFront();
	}
	
	objList.writeLogEntry("invokeNAMEHEREAdapterBringAppToFront result=%d", result->value.intValue);
	
	return true;
}

static bool invokeNAMEHEREAdapterResetLaunchState(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterResetLaunchState 0x%8.8x", (void*)obj);
	
	char *cbufTicket = NULL;
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
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
		
		result->value.intValue = namehereLauncher->ResetLaunchState();
	}
	
	objList.writeLogEntry("invokeNAMEHEREAdapterBringAppToFront result=%d", result->value.intValue);
	
	return true;
}

static bool invokeNAMEHEREAdapterGet_IsUpToDate(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsUpToDate 0x%8.8x", (void*)obj);
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
	
	result->type = NPVariantType_Bool;
	if (namehereLauncher == NULL){
		objList.writeLogEntry("ERROR: Launcher not found");
		result->value.boolValue = 0;
	} else {
		bool isUpToDate;
		int hr = namehereLauncher->IsUpToDate(&isUpToDate);
		objList.writeLogEntry("NAMEHEREAdapterGet_IsUpToDate HRESULT = %d", hr);
		result->value.boolValue = isUpToDate;
	}
	
	objList.writeLogEntry("invokeNAMEHEREAdapterGet_IsUpToDate result=%d", result->value.boolValue);
	
	return true;
}

static bool invokeNAMEHEREAdapterGetKeyValue(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterGetKeyValue");
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
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
		char *key = (char *)args[0].value.stringValue.UTF8_CHARACTERS;
		
		std::vector<char> zkey(args[0].value.stringValue.UTF8_LENGTH + 1);
		memcpy(&zkey[0], args[0].value.stringValue.UTF8_CHARACTERS, args[0].value.stringValue.UTF8_LENGTH);
		zkey[args[0].value.stringValue.UTF8_LENGTH] = 0;
		
		objList.writeLogEntry("invokeNAMEHEREAdapterGetKeyValue key = %s len = %d", &zkey[0], args[0].value.stringValue.UTF8_LENGTH);
		char strValue[2048] = {0};
		
		int hr = namehereLauncher->GetKeyValue(&zkey[0], strValue);
		if (hr > 0) // !Failed
		{
			int size = (uint32_t)strlen(strValue) + 1;
			result->value.stringValue.UTF8_LENGTH = size - 1;
			objList.writeLogEntry("invokeNAMEHEREAdapterGetKeyValue value = %s len = %d", strValue, result->value.stringValue.UTF8_LENGTH);
			
			result->value.stringValue.UTF8_CHARACTERS = (const NPUTF8 *) sObjList->npnfs->memalloc(size);
			strcpy((char *)result->value.stringValue.UTF8_CHARACTERS, strValue);
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
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
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
		
		std::vector<char> zkey(args[0].value.stringValue.UTF8_LENGTH + 1);
		memcpy(&zkey[0], args[0].value.stringValue.UTF8_CHARACTERS, args[0].value.stringValue.UTF8_LENGTH);
		zkey[args[0].value.stringValue.UTF8_LENGTH] = 0;
		
		std::vector<char> zvalue(args[1].value.stringValue.UTF8_LENGTH + 1);
		memcpy(&zvalue[0], args[1].value.stringValue.UTF8_CHARACTERS, args[1].value.stringValue.UTF8_LENGTH);
		zvalue[args[1].value.stringValue.UTF8_LENGTH] = 0;
		
		result->value.intValue = namehereLauncher->SetKeyValue(&zkey[0], &zvalue[0]);
		if (result->value.intValue != 0) {
			objList.writeLogEntry("NAMEHEREAdapterSetKeyValue returned %d", result->value.intValue);
		}
	}
	
	return true;
}


static bool invokeNAMEHEREAdapterDeleteKeyValue(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	objList.writeLogEntry("invokeNAMEHEREAdapterDeleteKeyValue");
	
	INAMEHERELauncher *namehereLauncher = sObjList->find(obj);
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
		
		std::vector<char> zkey(args[0].value.stringValue.UTF8_LENGTH + 1);
		memcpy(&zkey[0], args[0].value.stringValue.UTF8_CHARACTERS, args[0].value.stringValue.UTF8_LENGTH);
		zkey[args[0].value.stringValue.UTF8_LENGTH] = 0;
		
		result->value.intValue = namehereLauncher->DeleteKeyValue(&zkey[0]);// NAMEHEREAdapterDeleteKeyValue(namehereLauncher, &zkey[0]);
		
		//_launcher->DeleteKeyValue(%zkey[0]);
		
		if (result->value.intValue != 0) {
			objList.writeLogEntry("NAMEHEREAdapterDeleteKeyValue returned %d", result->value.intValue);
		}
	}
	return true;
}



typedef bool (*invokePtr)(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result);

static std::map<std::string, invokePtr> InitKnownMethods()
{
	std::map<std::string, invokePtr> result;
	
	// javascript interface method declaration
	result["HelloWorld"] = invokeNAMEHEREAdapterHelloWorld;
	result["Hello"] = invokeNAMEHEREAdapterHello;
	result["StartGame"] = invokeNAMEHEREAdapterStartGame;
	result["PreStartGame"] = invokeNAMEHEREAdapterPreStartGame;
	result["Get_InstallHost"] = invokeNAMEHEREAdapterGet_InstallHost;
	result["Update"] = invokeNAMEHEREAdapterUpdate;
	result["Put_AuthenticationTicket"] = invokeNAMEHEREAdapterPut_AuthenticationTicket;
	result["Get_GameStarted"] = invokeNAMEHEREAdapterGet_IsGameStarted;
	result["SetSilentModeEnabled"] = invokeNAMEHEREAdapterGet_SetSilentModeEnabled;
	result["SetStartInHiddenMode"] = invokeNAMEHEREAdapterGet_SetStartInHiddenMode;
	result["UnhideApp"] = invokeNAMEHEREAdapterGet_UnhideApp;
	result["BringAppToFront"] = invokeNAMEHEREAdapterBringAppToFront;
	result["ResetLaunchState"] = invokeNAMEHEREAdapterResetLaunchState;
	result["Get_IsUpToDate"] = invokeNAMEHEREAdapterGet_IsUpToDate;
	result["GetKeyValue"] = invokeNAMEHEREAdapterGetKeyValue;
	result["SetKeyValue"] = invokeNAMEHEREAdapterSetKeyValue;
	result["DeleteKeyValue"] = invokeNAMEHEREAdapterDeleteKeyValue;
	
	
	
	return result;
}

/**
 * Invoke GetInstallHost method from NAMEHERE ActiveX
 *
 */
bool invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	//gPlugin->invoke(obj, );
	objList.writeLogEntry("invoke");
	static std::map<std::string, invokePtr> methods = InitKnownMethods();
	
	char *name = sObjList->npnfs->utf8fromidentifier(methodName);
	if (name != NULL && methods[name] != NULL)
	{
		return methods[name](obj, args, argCount, result);
	} 
	else
	{
		// aim exception handling
		sObjList->npnfs->setexception(obj, "exception during invocation");
		return false;
	}
}

bool hasMethod(NPObject* obj, NPIdentifier methodName) 
{
	return true;
}

bool hasProperty(NPObject *obj, NPIdentifier propertyName) 
{
	return false;
}

bool getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) 
{
	return false;
}
