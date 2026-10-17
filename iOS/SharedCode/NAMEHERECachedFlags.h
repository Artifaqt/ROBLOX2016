//
//  NAMEHERECachedFlags.h
//  NAMEHEREMobile
//
//  Copyright (c) 2013 NAMEHERE. All rights reserved.
//

#pragma once

#include <Foundation/Foundation.h>

@interface NAMEHERECachedFlags : NSObject
{
}


- (BOOL) getBool:(NSString*)key withValue:(BOOL*)val;
- (BOOL) getInt:(NSString*)key withValue:(NSInteger*)val;
- (BOOL) getString:(NSString*)key withValue:(NSString*)val;

- (void) setBool:(NSString*)key withValue:(BOOL)val;
- (void) setInt:(NSString*)key withValue:(NSInteger)val;
- (void) setString:(NSString*)key withValue:(NSString*)val;


- (void) sync;

+(id) sharedInstance;

@end

