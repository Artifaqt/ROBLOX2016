//
//  PlayerThumbnailCell.m
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 9/23/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import "RBPlayerThumbnailCell.h"
#import "NAMEHERETheme.h"

#define FRIEND_AVATAR_SIZE CGSizeMake(110, 110)

@implementation RBPlayerThumbnailCell

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if(self)
    {
        [NAMEHERETheme applyRoundedBorderToView:self];
    }
    return self;
}

- (void)setFriendInfo:(RBXFriendInfo *)friendInfo
{
    _friendInfo = friendInfo;
    
    [NAMEHERETheme applyToFriendCellLabel:self.nameLabel];
    self.nameLabel.backgroundColor = [UIColor whiteColor];
    
    if(_friendInfo)
    {
        self.nameLabel.text = friendInfo.username;
        
        self.avatar.animateInOptions = RBXImageViewAnimateInAlways;
        [self.avatar loadAvatarForUserID:[friendInfo.userID integerValue] prefetchedURL:friendInfo.avatarURL urlIsFinal:friendInfo.avatarIsFinal withSize:FRIEND_AVATAR_SIZE completion:nil];
        
        NSString* imageName = friendInfo.isOnline ? @"User Online" : @"User Offline";
        [self.isOnlineMarker setImage:[UIImage imageNamed:imageName]];
    }
    else
    {
        self.nameLabel.text = @"";
        self.avatar.image = nil;
        self.isOnlineMarker.image = nil;
    }
}

@end
