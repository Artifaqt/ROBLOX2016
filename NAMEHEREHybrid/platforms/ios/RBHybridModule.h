//
//  RBHybridModule.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 2/5/15.
//  Copyright (c) 2015 NAMEHERE. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RBHybridCommand.h"

@interface RBHybridModule : NSObject

// Module properties
@property(strong, nonatomic) NSString* moduleID;

// Initialization function
-(instancetype)initWithModuleID:(NSString*)moduleID;

// Register a function to be executed from JS
-(void)registerFunction:(NSString*)functionName withSelector:(SEL)selector;

// Internal function to make the actual call.
// Override this function in your module if you need special function handling.
-(void)executeCommand:(RBHybridCommand*)command;


@end
