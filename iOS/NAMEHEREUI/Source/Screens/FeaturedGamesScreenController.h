//
//  FeaturedGamesScreenController.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 5/23/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import "RBBaseViewController.h"

@interface FeaturedGamesScreenController : RBBaseViewController<UIWebViewDelegate>

-(void)loadGames;
-(void)fetchSiteAlertBanner:(UIWebView*)webView;
@end
