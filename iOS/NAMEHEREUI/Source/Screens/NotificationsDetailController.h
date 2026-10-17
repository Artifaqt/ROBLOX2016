//
//  NotificationsDetailController.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 6/19/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"
#import "RBBaseViewController.h"

@interface NotificationCell : UITableViewCell
@property (strong, nonatomic) IBOutlet UIImageView* unreadIndicator;
@property (strong, nonatomic) IBOutlet UILabel *playerNameLabel;
@property (strong, nonatomic) IBOutlet UILabel *dateLabel;
@property (strong, nonatomic) IBOutlet UILabel *messageLabel;
@property (strong, nonatomic) RBXMessageInfo* messageData;
@property (nonatomic) BOOL isRead;
@end

@interface NotificationsDetailController : RBBaseViewController

- (void) loadData;
- (void) refreshMessages:(UIRefreshControl*)refreshControl;

@end
