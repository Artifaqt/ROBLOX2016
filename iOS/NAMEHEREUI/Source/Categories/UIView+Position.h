//
//  UIView+Position.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 9/17/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIView (Position)

@property (nonatomic, assign) CGPoint position;
@property (nonatomic, assign) CGSize  size;

@property (nonatomic, assign) CGFloat height;
@property (nonatomic, assign) CGFloat width;
@property (nonatomic, assign) CGFloat x;
@property (nonatomic, assign) CGFloat y;

@property (nonatomic, assign) CGFloat right;
@property (nonatomic, assign) CGFloat bottom;

- (void) centerInFrame:(CGRect)aFrame;
@end
