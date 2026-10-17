//
//  RBHybridWKWebView.h
//  NAMEHEREMobile
//
//  Created by Ashish Jain on 10/16/15.
//  Copyright © 2015 NAMEHERE. All rights reserved.
//

#import <WebKit/WebKit.h>

#import "RBHybridWebViewProtocol.h"
#import "WKWebView+RBXWKWebView.h"

@interface RBHybridWKWebView : WKWebView <RBHybridWebViewProtocol>

@end
