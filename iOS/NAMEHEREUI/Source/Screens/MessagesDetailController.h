//
//  MessagesDetailController.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 6/19/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"
#import "NAMEHEREImageView.h"
#import "RBBaseViewController.h"

@interface MessageCell : UITableViewCell
@property (strong, nonatomic) IBOutlet UIImageView* unreadIndicator;
@property (strong, nonatomic) IBOutlet NAMEHEREImageView *playerAvatar;
@property (strong, nonatomic) IBOutlet UILabel *playerNameLabel;
@property (strong, nonatomic) IBOutlet UILabel *dateLabel;
@property (strong, nonatomic) IBOutlet UILabel *messageLabel;
@property (strong, nonatomic) RBXMessageInfo* messageData;
@property (nonatomic) BOOL isRead;
@end

@interface MessagesDetailController : RBBaseViewController

@property (nonatomic) RBXMessageType typeOfMessages;

- (void) loadData;
- (void) refreshMessages:(UIRefreshControl*)refreshControl;
@end
