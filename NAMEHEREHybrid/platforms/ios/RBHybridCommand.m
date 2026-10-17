//
//  RBHybridCommand.m
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 2/5/15.
//  Copyright (c) 2015 NAMEHERE. All rights reserved.
//

#import "RBHybridCommand.h"
#import "RBHybridBridge.h"
#import "RBHybridWebView.h"

@implementation RBHybridCommand

-(void)executeCallback:(BOOL)success
{
    if(_callbackID)
    {
        [_originWebView executeCallback:_callbackID success:success withParams:nil];
    }
}

-(void)executeCallback:(BOOL)success withParams:(NSDictionary*)params
{
    if(_callbackID)
    {
        [_originWebView executeCallback:_callbackID success:(BOOL)success withParams:params];
    }
}

@end
