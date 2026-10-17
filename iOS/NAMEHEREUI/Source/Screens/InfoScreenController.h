//
//  InfoScreenController.h
//  NAMEHEREMobile
//
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NonRotatableViewController.h"

@interface InfoScreenController : NonRotatableViewController <UIWebViewDelegate>
@property (retain, nonatomic) IBOutlet UITextView *txtDeviceInfo;
@property (retain, nonatomic) IBOutlet UITextView *txtNAMEHEREInfo;
@end
