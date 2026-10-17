//
//  GameConsumableCollectionViewController.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 5/28/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NAMEHEREData.h"

@interface GameConsumableCollectionViewController : UIViewController

-(void) fetchGearForPlaceID:(NSString*)placeID gameTitle:(NSString*)gameTitle completion:(void(^)())completionHandler;
-(void) fetchPassesForPlaceID:(NSString*)placeID gameTitle:(NSString*)gameTitle completion:(void(^)())completionHandler;

@property(nonatomic, readonly) NSUInteger numItems;

@end
