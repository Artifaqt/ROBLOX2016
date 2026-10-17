//
//  HomeScreenController.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 5/23/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RBXEventReporter.h"

@interface HomeScreenController : UITabBarController<UITabBarControllerDelegate>

-(RBXAnalyticsCustomData) getCurrentTabContext;

@end
