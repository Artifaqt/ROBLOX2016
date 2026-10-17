//
//  CrossFadeSegue.m
//  NAMEHEREMobile
//
//  Created by Kyler Mulherin on 10/29/15.
//  Copyright © 2015 NAMEHERE. All rights reserved.
//

#import "CrossFadeSegue.h"

@implementation CrossFadeSegue

-(void) perform
{
    //grab our views
    UIViewController* source = self.sourceViewController;
    UIViewController* destination = self.destinationViewController;
    
    //initialize a view to fade in
    destination.view.hidden = NO;
    destination.view.alpha = 0.0;
    
    //animate the transition
    [UIView animateWithDuration:0.5
                          delay:0.0
                        options:UIViewAnimationOptionCurveEaseIn
                     animations:^{
                         //fade in the next screen
                         destination.view.alpha = 1.0;
                     } completion:^(BOOL finished) {
                         [source.navigationController pushViewController:destination animated:NO];
                     }];
}

@end
