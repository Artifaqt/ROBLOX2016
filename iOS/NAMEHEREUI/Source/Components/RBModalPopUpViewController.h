//
//  RBModalPopUpViewController.h
//  NAMEHEREMobile
//
//  Created by Kyler Mulherin on 10/15/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "NAMEHERETheme.h"

@interface RBModalPopUpViewController : UIViewController <UIGestureRecognizerDelegate>

-(void) disableTapRecognizer;
-(void) shouldAddCloseButton:(BOOL)shouldAdd;
-(void) shouldApplyModalTheme:(BOOL)shouldApply;
@end
