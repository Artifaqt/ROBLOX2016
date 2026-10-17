#include <stdio.h>
#include <comdef.h>

#include "NAMEHEREProxy_i.h"


const IID IID_ILauncher =
{0x699F0898,0xB7BC,0x4DE5,{0xAF,0xEE,0x0E,0xC3,0x8A,0xD4,0x22,0x40}};

#ifdef _WIN64
	const CLSID CLSID_Launcher =
	{0xDEE03C2B,0x0C0C,0x41A9,{0x98,0x77,0xFD,0x4B,0x4D,0x7B,0x6E,0xA3}};
#else
	const CLSID CLSID_Launcher =
	{0x76D50904,0x6780,0x4c8b,{0x89,0x86,0x1A,0x7E,0xE0,0xB1,0x71,0x6D}};
#endif

//=================================================================================
//=================COM Synchronization routing functions========================================
//=================================================================================

bool isComInitialized = false;

void StartCom(void){
	HRESULT	hr;
	if( !isComInitialized ){
		hr = CoInitialize(0);
		if( SUCCEEDED(hr) ){
			isComInitialized = true;
		}
	}
}

void StopCom(void){
	if( isComInitialized ){
		CoUninitialize();
		isComInitialized = false;
	}
}

bool isComStarted(void){
	return isComInitialized;
}

//=================================================================================
//=================String routing functions========================================
//=================================================================================

/**
* Conversion from ASCII to WideChar
*
*/

int Str2Wc( char* pBuf, WCHAR* pwcBuf, int bufLen){
	if( pwcBuf == NULL || pBuf == NULL )
		return -1;

	// If zero then assume the user has allocated enough
	if( bufLen == 0 ){
		bufLen = (int)( strlen(pBuf) + 1 );
	}
	if( MultiByteToWideChar( CP_ACP, 0, pBuf, -1, pwcBuf, bufLen) == 0 ){
		return -1;
	}
	pwcBuf[strlen(pBuf)] = 0;
	return 0;
}
/**
* Conversion from WideChar to ASCII
*
*/
int Wc2Str(WCHAR* pwcBuf, char* pBuf, int bufLen){
	if( pwcBuf == NULL || pBuf == NULL )
		return -1;

// If zero then assume the user has allocated enough
	if( bufLen == 0 ){
		bufLen = (int)( wcslen( pwcBuf) + 1 );
	}
	if( WideCharToMultiByte( CP_ACP, 0, pwcBuf, -1, pBuf, bufLen, NULL, NULL) == 0 ){
		return -1;
	}
	pBuf[wcslen(pwcBuf)] = 0;
	return 0;
}

//=================================================================================
//=================NAMEHERE ActiveX functions call===================================
//=================================================================================

/**
* Create NAMEHERE ActiveX instance
*
*/

ILauncher * GetINAMEHERELauncherInstance(void){
	// Declare and HRESULT and a pointer to the NAMEHEREProxy interface
	HRESULT	hr;
	ILauncher * INAMEHERELauncher;
	if( isComStarted() ){
		hr = CoCreateInstance( CLSID_Launcher, NULL, CLSCTX_INPROC_SERVER,
						IID_ILauncher, (void**) &INAMEHERELauncher);

		if( !SUCCEEDED(hr) ){
			INAMEHERELauncher=NULL;
		}
	}
	return INAMEHERELauncher;

}

/**
* Invoke HelloWorld method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterHelloWorld(ILauncher *iINAMEHERELauncher){
	HRESULT	hr = iINAMEHERELauncher->HelloWorld();
	return hr;
}

/**
* Invoke Hello method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterHello(ILauncher *iINAMEHERELauncher,char *retMessage){
	HRESULT			hr;
	BSTR hhh;

	hr = -1;
	hr = iINAMEHERELauncher->Hello( &hhh );
	if( SUCCEEDED(hr) ){
		Wc2Str( hhh, retMessage, 0 );
	}
	else {
		strcpy( retMessage, "INAMEHERELauncher->Hello() Error occured !" );
	}

	return hr;
}
/**
* Invoke StartGame method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterStartGame(ILauncher *iINAMEHERELauncher,char *cbufAuth, char *cbufScript ){
	HRESULT			hr;
	_bstr_t authenticationUrl(cbufAuth);
	_bstr_t script(cbufScript);

	hr = -1;
	hr = iINAMEHERELauncher->StartGame( authenticationUrl.GetBSTR(), 
									script.GetBSTR() );

	return hr;
}

/**
* Invoke PreStartGame method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterPreStartGame(ILauncher *iINAMEHERELauncher){
	HRESULT hr = -1;

	hr = iINAMEHERELauncher->PreStartGame();

	return hr;
}

/**
* Invoke GetInstallHost method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterGet_InstallHost(ILauncher *iINAMEHERELauncher,char *cbufHost){
	HRESULT			hr;
	BSTR host;
	hr = -1;
	hr = iINAMEHERELauncher->get_InstallHost( &host );

	if( SUCCEEDED(hr) ){
		Wc2Str( host, cbufHost, 0 );
	}
	else {
		strcpy( cbufHost, "INAMEHERELauncher->get_InstallHost() Error occured !" );
	}


	return hr;
}

/**
* Invoke GetVersion method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterGet_Version(ILauncher *iINAMEHERELauncher,char *cbufHost) {
	HRESULT			hr;
	BSTR host;
	hr = -1;
	hr = iINAMEHERELauncher->get_Version( &host );

	if( SUCCEEDED(hr) ){
		Wc2Str( host, cbufHost, 0 );
	}
	else {
		strcpy( cbufHost, "INAMEHERELauncher->get_Version() Error occured !" );
	}


	return hr;
}

/**
* Invoke Update method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterUpdate(ILauncher *iINAMEHERELauncher){
	HRESULT			hr;
	hr = iINAMEHERELauncher->Update();

	return hr;
}

/**
* Invoke PutAuthentificationTicket method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterPut_AuthenticationTicket(ILauncher *iINAMEHERELauncher,char *cbufTicket ){
	HRESULT			hr;
	_bstr_t ticket(cbufTicket);

	hr = -1;
	hr = iINAMEHERELauncher->put_AuthenticationTicket(ticket.GetBSTR());

	return hr;
}

/**
* Invoke IsGameStarted method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterGet_IsGameStarted(ILauncher *iINAMEHERELauncher, bool *isStarted){
	HRESULT			hr;
	VARIANT_BOOL started;

	hr = -1;
	hr = iINAMEHERELauncher->get_IsGameStarted(&started);
	(*isStarted) = (started == VARIANT_TRUE);

	return hr;
}

/**
* Invoke SetSilentModeEnabled method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterGet_SetSilentModeEnabled(ILauncher *iINAMEHERELauncher, bool silendMode){
	HRESULT			hr;
	VARIANT_BOOL silent = silendMode ? VARIANT_TRUE : VARIANT_FALSE;

	hr = -1;
	hr = iINAMEHERELauncher->SetSilentModeEnabled(silent);

	return hr;
}

/**
* Invoke UnhideApp method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterGet_UnhideApp(ILauncher *iINAMEHERELauncher)
{
	HRESULT			hr;

	hr = -1;
	hr = iINAMEHERELauncher->UnhideApp();

	return hr;
}

/**
* Invoke BringAppToFront method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterBringAppToFront(ILauncher *iINAMEHERELauncher)
{
	HRESULT			hr;

	hr = -1;
	hr = iINAMEHERELauncher->BringAppToFront();

	return hr;
}

/**
* Invoke ResetLaunchState method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterResetLaunchState(ILauncher *iINAMEHERELauncher)
{
	HRESULT			hr;

	hr = -1;
	hr = iINAMEHERELauncher->ResetLaunchState();

	return hr;
}

/**
* Invoke SetEditMode method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterSetEditMode(ILauncher *iINAMEHERELauncher)
{
	HRESULT			hr;

	hr = -1;
	hr = iINAMEHERELauncher->SetEditMode();

	return hr;
}

/**
* Invoke SetStartInHiddenMode method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterGet_SetStartInHiddenMode(ILauncher *iINAMEHERELauncher, bool hiddenMode){
	HRESULT			hr;
	VARIANT_BOOL hidden = hiddenMode ? VARIANT_TRUE : VARIANT_FALSE;

	hr = -1;
	hr = iINAMEHERELauncher->SetStartInHiddenMode(hidden);

	return hr;
}

/**
* Invoke get_IsUpToDate method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterGet_IsUpToDate(ILauncher *iINAMEHERELauncher, bool *isUpToDate){
	int hr;
	VARIANT_BOOL upToDate = VARIANT_FALSE;

	hr = -1;
	hr = iINAMEHERELauncher->get_IsUpToDate(&upToDate);

	*isUpToDate = (upToDate == VARIANT_TRUE);
	
	return hr;
}

/**
* Invoke GetKeyValue method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterGetKeyValue(ILauncher *iINAMEHERELauncher, char *key, char *value)
{
	int hr = -1;
	_bstr_t bstrKey(key);
	BSTR bstrValue;
	VARIANT_BOOL exists = VARIANT_FALSE;

	hr = iINAMEHERELauncher->GetKeyValue(bstrKey, &bstrValue);
	if (SUCCEEDED(hr))
	{
		Wc2Str(bstrValue, value, 0);
	} else {
		value = NULL;
	}

	return hr;
}

/**
* Invoke SetKeyValue method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterSetKeyValue(ILauncher *iINAMEHERELauncher, char *key, char *value)
{
	_bstr_t bstrKey(key);
	_bstr_t bstrValue(value);

	return iINAMEHERELauncher->SetKeyValue(bstrKey, bstrValue);
}

/**
* Invoke DeleteKeyValue method from NAMEHERE ActiveX
*
*/
int NAMEHEREAdapterDeleteKeyValue(ILauncher *iINAMEHERELauncher, char *key)
{
	_bstr_t bstrKey(key);

	return iINAMEHERELauncher->DeleteKeyValue(bstrKey);
}