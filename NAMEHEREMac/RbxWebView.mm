//
//  RbxWebView.m
//  MacClient
//
//  Created by Ben Tkacheff on 9/25/13.
//
//

#import "RbxWebView.h"
#import "NAMEHEREPlayerAppDelegate.h"

#include "NAMEHEREView.h"
#include "v8datamodel/GuiService.h"
#include "v8datamodel/Datamodel.h"

FASTSTRING(ClientExternalBrowserUserAgent)

@implementation RbxWebView

@synthesize webView;

-(id) initWithWindowNibName:(NSString*) name
{
    if(self = [super initWithWindowNibName:name])
    {
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(windowWillClose:)
                                                     name:NSWindowWillCloseNotification
												   object:nil];
    }
    return self;
}

-(void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [super dealloc];
}


static void doInformDataModelUrlWindowClose(RBX::DataModel* dataModel)
{
    if (RBX::GuiService* guiService = dataModel->find<RBX::GuiService>())
        guiService->urlWindowClosed();
}

- (void) informDataModelUrlWindowClose:(RBX::DataModel*) dataModel
{
    if(dataModel)
        dataModel->submitTask(boost::bind(&doInformDataModelUrlWindowClose,dataModel), RBX::DataModelJob::Write);
}

- (void)windowDidLoad
{
    NSString *versionString;
    NSDictionary * sv = [NSDictionary dictionaryWithContentsOfFile:@"/System/Library/CoreServices/SystemVersion.plist"];
    versionString = [sv objectForKey:@"ProductVersion"];
    
    [self.webView setCustomUserAgent:[NSString stringWithFormat:@"Mozilla/5.0 (Macintosh; Intel Mac OS X %@) AppleWebKit/536.25 (KHTML, like Gecko) %s",versionString, FString::ClientExternalBrowserUserAgent.c_str()]];
}


- (void)windowWillClose:(NSNotification *)notification
{
    NSWindow* closingWindow = (NSWindow*)notification.object;
    if (closingWindow && closingWindow == self.window )
    {
        NAMEHEREView* namehereView = NULL;
        
        NAMEHEREPlayerAppDelegate *appDelegate = (NAMEHEREPlayerAppDelegate*)[NSApp delegate];
        if(!appDelegate)
            return;
        if(!appDelegate.namehereView)
            return;
        
        namehereView = appDelegate.namehereView;
        
        if (namehereView)
        {
            [self informDataModelUrlWindowClose:namehereView->getDataModel().get()];
        }
        
        [self.window resignFirstResponder];
        [self.window resignMainWindow];

        [self.webView setResourceLoadDelegate:nil];
        
        [self autorelease];
    }
}

@end
