//
//  NativeSearchNavItem.h
//  NAMEHEREMobile
//
//  Created by Kyler Mulherin on 11/10/14.
//  Modified from code by Ariel Lichten.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"

@interface NativeSearchNavItem : UIBarButtonItem

typedef NS_ENUM(NSInteger, SearchStatus)
{
    SearchStatusOpen,
    SearchStatusClosed
};

- (id)initWithSearchType:(SearchResultType)searchType
            andContainer:(UIViewController*)target
             compactMode:(BOOL)compactMode;

@end
