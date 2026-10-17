//
//  SocialSignUpViewController.h
//  NAMEHEREMobile
//
//  Created by Kyler Mulherin on 9/10/15.
//  Copyright © 2015 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NonRotatableViewController.h"
#import "RBValidTextField.h"
#import "NAMEHEREImageView.h"

@interface SocialSignUpViewController : NonRotatableViewController <UIGestureRecognizerDelegate>

@property IBOutlet NAMEHEREImageView* imgIdentity;
@property IBOutlet RBValidTextField* txtUsername;
@property IBOutlet UILabel* lblSelectUserName;
@property IBOutlet UILabel* lblNameDescription;
@property IBOutlet UILabel* lblAlreadyHaveAccount;
@property IBOutlet UILabel* lblAlmostDone;
@property IBOutlet UIButton* btnAccept;
@property IBOutlet UIButton* btnCancel;
@property IBOutlet UIView* whiteView;
@property IBOutlet UIView* shadowContainer;

-(IBAction) createAccount:(id)sender;
-(IBAction) cancelSignUp:(id)sender;

@end
