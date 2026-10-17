//
//  RBProfileViewController.h
//  NAMEHEREMobile
//
//  Created by Christian Hresko on 6/6/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RBBaseViewController.h"
#import "NAMEHEREData.h"

@interface RBProfileViewController : RBBaseViewController<UICollectionViewDelegate, UICollectionViewDataSource>

@property (strong, nonatomic) NSNumber* userId;
@property (strong, nonatomic) RBXUserProfileInfo* profile;

- (void)updateFriendshipButton;
- (void)updateFollowButton;
@end