//
//  RBMessageComposeScreenController.h
//  NAMEHEREMobile
//
//  Created by alichtin on 9/18/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"

@interface RBMessageComposeScreenController : UIViewController

- (void) replyToMessage:(RBXMessageInfo*)message;

- (void) sendMessageToUser:(NSNumber*)userID name:(NSString*)userName;

@end
