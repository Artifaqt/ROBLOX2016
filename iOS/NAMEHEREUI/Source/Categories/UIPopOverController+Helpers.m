//
//  UIPopOverController+Helpers.m
//  NAMEHEREMobile
//
//  Created by Ashish Jain on 9/4/15.
//  Copyright (c) 2015 NAMEHERE. All rights reserved.
//

#import "UIPopOverController+Helpers.h"

@implementation UIPopoverController (Helpers)

- (void) dismissPopoverAnimated:(BOOL)animated completionHandler:(void(^)(void))completionHandler {
    [CATransaction begin];
    [CATransaction setCompletionBlock:completionHandler];
    [self dismissPopoverAnimated:animated];
    [CATransaction commit];
}

@end
