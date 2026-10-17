//
//  RBHybridBridge.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 2/5/15.
//  Copyright (c) 2015 NAMEHERE. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "RBHybridWebViewProtocol.h"

@class RBHybridWebView;
@class RBHybridModule;


@interface RBHybridBridge : NSObject

+ (RBHybridBridge*) sharedInstance;

-(void)registerWebView:(UIView <RBHybridWebViewProtocol> *)webview;
-(void)unRegisterWebView:(UIView <RBHybridWebViewProtocol> *)webview;

-(RBHybridWebView*)getWebViewForID:(NSString*)webviewID;
-(RBHybridModule*)getModuleForID:(NSString*)moduleID;

@end
