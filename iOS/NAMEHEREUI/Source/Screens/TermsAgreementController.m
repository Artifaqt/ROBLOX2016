//
//  TermsAgreementController.m
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 5/22/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import "TermsAgreementController.h"
#import "NAMEHERETheme.h"
#import "NAMEHEREInfo.h"
#import "UIView+Position.h"

@interface TermsAgreementController ()

@end

@implementation TermsAgreementController
{
    IBOutlet UIWebView *_webView;
    IBOutlet UINavigationBar *_navBar;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [NAMEHERETheme applyToModalPopupNavBar:_navBar];
    
    NSURL *url = [NSURL URLWithString:self.url];
    NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];
    [_webView loadRequest:requestObj];
    [_webView setScalesPageToFit:YES];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [_webView stopLoading];
}


- (IBAction)closeButtonTouched:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}
@end
