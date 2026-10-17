//
//  GameThumbnailCell.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 5/27/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RBXGameData;
@class NAMEHEREImageView;

@interface CarouselThumbnailCell : UICollectionViewCell

-(void) setGameData:(RBXGameData*)data;

@end
