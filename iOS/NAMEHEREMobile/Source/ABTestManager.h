//
//  ABTestManager.h
//  NAMEHEREMobile
//
//  Created by Kyler Mulherin on 3/29/15.
//  Copyright (c) 2015 NAMEHERE. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NAMEHEREData.h"

@interface ABTestManager : NSObject

//static instance
+(id)sharedInstance;

-(void) fetchExperimentsForBrowserTracker;
-(void) fetchExperimentsForUser:(NSNumber*)userId;

- (void) removeStoredExperiments;

//Accessors
//Enrollment tests
-(BOOL) IsPartOfTestMobileGuestMode;
-(BOOL) IsPartOfTestGuestFlavorMode;

//App Behavioral tests
-(BOOL) IsInTestMobileGuestMode;
-(BOOL) IsInTestGuestFlavorMode;
@end
