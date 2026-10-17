//
//  GamePreviewScreenController.h
//  NAMEHEREMobile
//
//  Created by alichtin on 5/30/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"

@interface GamePreviewScreenController : UIViewController<UITextViewDelegate, UIScrollViewDelegate>

@property (strong, nonatomic) RBXGameData* gameData;

@end
