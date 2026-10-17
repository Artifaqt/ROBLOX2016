//
//  RBVotesView.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 10/1/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"

@interface RBVotesView : UIView

- (void) setVotesForGame:(RBXGameData*)gameData;
- (void) setVotesForPass:(RBXGamePass*)gamePass;

@end
