//
//  GamePreviewDescriptionViewController.h
//  NAMEHEREMobile
//
//  Created by Kyler Mulherin on 10/6/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RBModalPopUpViewController.h"


@interface GamePreviewDescriptionViewController : RBModalPopUpViewController <UIScrollViewDelegate>

@property NSString* gameDescription;
@property UILabel* lblDescription;
@property UIScrollView* svScroll;

-(id) initWithDescription:(NSString*)aDescription;
@end
