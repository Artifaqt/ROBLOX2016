//
//  RBBirthdayPicker.h
//  NAMEHEREMobile
//
//  Created by Kyler Mulherin on 7/15/15.
//  Copyright (c) 2015 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface RBBirthdayPicker : UIView <UIPickerViewDelegate, UIPickerViewDataSource>

@property (nonatomic, getter=getBirthday, readonly)     NSString* playerBirthday;
@property (nonatomic, getter=getWasBornToday, readonly) BOOL wasBornToday;

-(BOOL) isValid;
-(BOOL) userUnder13;
-(void) setValidationBlock:(void(^)())callback;

-(void) markAsValid;
-(void) markAsNormal;
-(void) markAsInvalid;


- (BOOL)becomeFirstResponder;
- (BOOL)resignFirstResponder;
@end
