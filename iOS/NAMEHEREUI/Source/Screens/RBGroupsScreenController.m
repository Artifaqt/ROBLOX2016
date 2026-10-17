//
//  RBGroupsMasterController.m
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 9/3/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import "RBGroupsScreenController.h"
#import "NAMEHERETheme.h"
#import "NAMEHEREInfo.h"
#import "Flurry.h"

#define GS_openRobux        @"GROUPS SCREEN - Open Robux"
#define GS_openBuildersClub @"GROUPS SCREEN - Open Builders Club"
#define GS_openSettings     @"GROUPS SCREEN - Open Settings"
#define GS_openLogout       @"GROUPS SCREEN - Open Logout"
#define GS_groupSearch      @"GROUPS SCREEN - Group Search"
#define GS_groupPageLink    @"GROUPS SCREEN - Group Page Link"
#define GS_openExternalLink @"GROUPS SCREEN - Open External Link"
#define GS_openProfile      @"GROUPS SCREEN - Open Profile"
#define GS_openGameDetail   @"GROUPS SCREEN - Open Game Detail"
#define GS_launchGame       @"GROUPS SCREEN - Launch Game"

@interface RBGroupsScreenController ()

@end

@implementation RBGroupsScreenController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationItem.title = NSLocalizedString(@"GroupsWord", nil);
    
    BOOL tablet = [NAMEHEREInfo thisDeviceIsATablet];
    self.url = [[NAMEHEREInfo getBaseUrl] stringByAppendingString:(tablet == YES) ? @"My/Groups.aspx" : @"my-groups"];
    
    if ([NAMEHEREInfo thisDeviceIsATablet])
        [self addRobuxIconWithFlurryEvent:GS_openRobux
                 andBCIconWithFlurryEvent:GS_openBuildersClub];
    
    [self setFlurryPageLoadEvent:GS_groupPageLink];
    [self setFlurryGameLaunchEvent:GS_launchGame];
    [self setFlurryEventsForExternalLinkEvent:GS_openExternalLink
                              andWebViewEvent:nil
                          andOpenProfileEvent:GS_openProfile
                       andOpenGameDetailEvent:GS_openGameDetail];
}

@end
