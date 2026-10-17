#ifndef _ROLBOX_ADAPTER_H_
#define _ROLBOX_ADAPTER_H_

void StartCom(void);
void StopCom(void);
bool isComStarted(void);

ILauncher * GetINAMEHERELauncherInstance(void);

int NAMEHEREAdapterHelloWorld(ILauncher *iINAMEHERELauncher);
int NAMEHEREAdapterHello(ILauncher *iINAMEHERELauncher,char *retMessage);
int NAMEHEREAdapterStartGame(ILauncher *iINAMEHERELauncher,char *cbufAuth, char *cbufScript );
int NAMEHEREAdapterPreStartGame(ILauncher *iINAMEHERELauncher);
int NAMEHEREAdapterGet_InstallHost(ILauncher *iINAMEHERELauncher,char *cbufHost);
int NAMEHEREAdapterGet_Version(ILauncher *iINAMEHERELauncher,char *cbufHost);
int NAMEHEREAdapterUpdate(ILauncher *iINAMEHERELauncher);
int NAMEHEREAdapterPut_AuthenticationTicket(ILauncher *iINAMEHERELauncher,char *cbufTicket );
int NAMEHEREAdapterGet_IsGameStarted(ILauncher *iINAMEHERELauncher, bool *isStarted);
int NAMEHEREAdapterGet_SetSilentModeEnabled(ILauncher *iINAMEHERELauncher, bool silendMode);
int NAMEHEREAdapterGet_UnhideApp(ILauncher *iINAMEHERELauncher);
int NAMEHEREAdapterBringAppToFront(ILauncher *iINAMEHERELauncher);
int NAMEHEREAdapterResetLaunchState(ILauncher *iINAMEHERELauncher);
int NAMEHEREAdapterSetEditMode(ILauncher *iINAMEHERELauncher);
int NAMEHEREAdapterGet_SetStartInHiddenMode(ILauncher *iINAMEHERELauncher, bool hiddenMode);
int NAMEHEREAdapterGet_IsUpToDate(ILauncher *iINAMEHERELauncher, bool *isUpToDate);
int NAMEHEREAdapterGetKeyValue(ILauncher *iINAMEHERELauncher, char *key, char *value);
int NAMEHEREAdapterSetKeyValue(ILauncher *iINAMEHERELauncher, char *key, char *value);
int NAMEHEREAdapterDeleteKeyValue(ILauncher *iINAMEHERELauncher, char *key);

#endif

