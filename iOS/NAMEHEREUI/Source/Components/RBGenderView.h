//
//  RBGenderView.h
//  NAMEHEREMobile
//
//  Created by Kyler Mulherin on 7/13/15.
//  Copyright (c) 2015 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SignupVerifier.h"

@interface RBGenderView : UIView

@property (nonatomic, setter=setGender:) Gender playerGender;

-(void) setTouchBlock:(void (^)())block;

@end
