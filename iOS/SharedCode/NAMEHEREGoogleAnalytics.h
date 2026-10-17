//
//  NAMEHEREGoogleAnalytics.h
//  NAMEHEREMobile
//
//  Created by Ganesh Agrawal on 11/13/12.
//  Copyright (c) 2012 NAMEHERE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NAMEHEREGoogleAnalytics : NSObject
{}

+(void) startup;
+(void) setPageViewTracking:(NSString*)url;
+(void) setEventTracking:(NSString*)category withAction:(NSString*)action withValue:(NSInteger)value;
+(void) setEventTracking:(NSString*)category withAction:(NSString*)action withLabel:(NSString*)label withValue:(NSInteger)value;
+(void) setCustomVariableWithLabel:(NSString*)label withValue:(NSString*)value;

// debug counters
+(void) debugCountersPrint;
+(void) debugCounterIncrement:(NSString *)label;

@end
