//
//  RBXUIUtil.h
//  NAMEHEREMobile
//
//  Created by Ashish Jain on 9/4/15.
//  Copyright (c) 2015 NAMEHERE. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface RBXUIUtil : NSObject

+ (void) increaseTappableAreaOfButton:(UIButton *)button;

+ (void) view:(UIView *)view setOrigin:(CGPoint)newOrigin;
+ (void) view:(UIView *)view setSize:(CGSize)newSize;

@end
