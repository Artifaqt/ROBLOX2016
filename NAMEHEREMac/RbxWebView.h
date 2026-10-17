//
//  RbxWebView.h
//  MacClient
//
//  Created by Ben Tkacheff on 9/25/13.
//
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

typedef enum
{
    NAMEHERE_APP_SHUTDOWN_CODE_LEAVE_GAME,
    NAMEHERE_APP_SHUTDOWN_CODE_WINDOW_CLOSE,
    NAMEHERE_APP_SHUTDOWN_CODE_QUIT
} NAMEHEREAppShutdownCode;

typedef enum
{
    GAMETYPE_PLAY,
    GAMETYPE_PLAY_PROTOCOL
} NAMEHEREGameType;

typedef enum
{
    SUCCESS,
    FAILED,
    RETRY,
    GAME_FULL,
    USER_LEFT,
} RequestPlaceInfoResult;

@interface RbxWebView : NSWindowController
{
    WebView *webView;
}

-(id) initWithWindowNibName:(NSString*) name;
-(void) dealloc;

@property (weak) IBOutlet WebView *webView;

@end
