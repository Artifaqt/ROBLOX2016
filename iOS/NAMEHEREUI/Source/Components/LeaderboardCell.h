//
//  LeaderboardCell.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 6/6/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"
#import "NAMEHEREImageView.h"

@interface LeaderboardCell : UITableViewCell

@property (strong, nonatomic) IBOutlet UILabel* rankLabel;
@property (strong, nonatomic) IBOutlet UILabel *titleLabel;
@property (strong, nonatomic) IBOutlet UILabel *subtitleLabel;
@property (strong, nonatomic) IBOutlet UILabel *pointsLabel;
@property (strong, nonatomic) IBOutlet NAMEHEREImageView *avatar;

- (void) showPlayerInfo:(RBXLeaderboardEntry*)data;
- (void) showClanInfo:(RBXLeaderboardEntry*)data;

@end
