//
//  GameSortResultsScreenController.h
//  NAMEHEREMobile
//
//  Created by alichtin on 5/30/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RBBaseViewController.h"

@interface GameSortResultsScreenController : RBBaseViewController

@property(strong, nonatomic) NSNumber* selectedSort;
@property (nonatomic, strong) RBXGameGenre *selectedGenre;
@property(strong, nonatomic) NSNumber *playerID;

+ (instancetype) gameSortResultsScreenControllerWithSort:(RBXGameSort *)sort genre:(RBXGameGenre *)genre playerID:(NSNumber *)playerID;

@end
