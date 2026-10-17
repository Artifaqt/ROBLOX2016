//
//  RBResetPasswordControllerViewController.m
//  NAMEHEREMobile
//
//  Created by Christian Hresko on 6/26/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import "RBResetPasswordViewController.h"
#import "NAMEHEREInfo.h"

@interface RBResetPasswordViewController ()

@end

@implementation RBResetPasswordViewController

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    return YES;
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch {
    return NO;
}

@end
