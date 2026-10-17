//
//  RBTabBarController.h
//  NAMEHEREMobile
//
//  Created by Ashish Jain on 12/8/15.
//  Copyright © 2015 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RBXEventReporter.h"

@interface RBTabBarController : UITabBarController

-(RBXAnalyticsCustomData) getCurrentTabContext;

@end
