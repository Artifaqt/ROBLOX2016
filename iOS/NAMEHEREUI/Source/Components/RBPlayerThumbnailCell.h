//
//  PlayerThumbnailCell.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 9/23/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "NAMEHEREImageView.h"
#import "NAMEHEREData.h"

@interface RBPlayerThumbnailCell : UICollectionViewCell

@property(strong, nonatomic) RBXFriendInfo* friendInfo;

@property(weak, nonatomic) IBOutlet NAMEHEREImageView* avatar;
@property(weak, nonatomic) IBOutlet UILabel* nameLabel;
@property(weak, nonatomic) IBOutlet UIImageView* isOnlineMarker;

@end
