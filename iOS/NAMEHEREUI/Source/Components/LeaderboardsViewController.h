//
//  LeaderboardsViewController.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 6/16/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"

@interface LeaderboardsViewController : UIViewController

@property (nonatomic) RBXLeaderboardsTimeFilter timeFilter;
@property (strong, nonatomic) NSNumber* distributorID;

// Callback
@property (copy, nonatomic) void (^onLeaderboardsLoaded)(BOOL hasLeaderboards);

- (void) updateLeaderboards;

@end
