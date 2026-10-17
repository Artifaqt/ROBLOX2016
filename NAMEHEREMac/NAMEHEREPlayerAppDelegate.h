//
//  SimplePlayerMacAppDelegate.h
//  SimplePlayerMac
//
//  Created by Tony on 10/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//
#pragma once

#import <Cocoa/Cocoa.h>
#import <Breakpad.h>

#import "NAMEHEREOgreView.h"

#include "rbx/signal.h"
#import "RbxWebView.h"
#import "RBXWindow.h"

class NAMEHEREView;

namespace RBX
{
    class DataModel;
}

@interface NAMEHEREPlayerAppDelegate : NSResponder <NSWindowDelegate>
{
    RBXWindow *window;
	NSView *mainView;
    RbxWebView *rbxWebView;
	NAMEHEREOgreView *ogreView;
	NAMEHEREView *namehereView;
	NSMenu *mainMenu;
	BOOL running;
	BOOL quitOnLeave;
	NAMEHEREAppShutdownCode shutdownCode;
	BreakpadRef breakpad;
    rbx::signals::scoped_connection openUrlConnection;
    rbx::signals::scoped_connection closeUrlConnection;
    NAMEHEREGameType gameType;
}

-(id) init;

- (BOOL) checkUpdater:(BOOL) showUpdateOptionsDialog;

-(void)marshallFunction:(NSEvent *)evt;

-(void)addLogFile:(NSString *)file;
-(void)addBreakPadKeyValue:(NSString *)breakPadKey withValue:(NSString *)value;
-(void)addDbgInfoToBreakPad;

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification;
-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;
-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
-(void)applicationWillTerminate:(NSNotification *)aNotification;

-(void)windowWillClose:(NSNotification *)notification;

-(void)ShutdownDataModel;
-(void)leaveGame;
-(void)handleLeaveGame;
-(BOOL)requestShutdownClient:(NAMEHEREAppShutdownCode)code;
-(void)handleShutdownClient;

-(void)handleToggleFullScreen;
-(bool)inFullScreenMode;

-(void)teleport:(NSString *)ticket withAuthentication:(NSString *)url withScript:(NSString *)script;
-(void)StartGame:(NSString *)ticket withAuthentication:(NSString *)url withScript:(NSString *)script;

bool requestPlaceInfo(int testPlaceID, std::string& authenticationUrl, std::string& ticket, std::string& scriptUrl);
RequestPlaceInfoResult requestPlaceInfo(const std::string& placeLauncherUrl, std::string& authenticationUrl, std::string& ticket, std::string& scriptUrl);

-(void) openUrlWindow:(std::string) url;

- (IBAction)crash:(id)sender;

-(void) setupGameServices;
-(void) setupDataModelServices:(RBX::DataModel*) dataModel;

+(void) reportRenderViewInitError:(const char *)message;

@property (assign) IBOutlet RBXWindow *window;
@property (assign) IBOutlet NSView *mainView;

@property (readwrite, assign) NAMEHEREView *namehereView;
@property (assign) IBOutlet NAMEHEREOgreView *ogreView;
@property (assign) IBOutlet NSMenu *mainMenu;

@end
