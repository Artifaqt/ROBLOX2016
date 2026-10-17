//
//  GameSearchResultCell.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 6/10/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "NAMEHEREData.h"

@interface GameSearchResultCell : UICollectionViewCell

@property(strong, nonatomic) RBXGameData* gameData;

+(CGSize) getCellSize;
+(NSString*) getNibName;

@end
