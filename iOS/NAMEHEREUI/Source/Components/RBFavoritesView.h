//
//  RBFavoritesView
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 10/1/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"

@interface RBFavoritesView : UIView

- (void) setFavoritesForGame:(RBXGameData*)gameData;
- (void) setFavoritesForPass:(RBXGamePass*)gamePass;
- (void) setFavoritesForGear:(RBXGameGear*)gameGear;

@end
