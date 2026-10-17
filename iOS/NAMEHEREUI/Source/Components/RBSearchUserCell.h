//
//  RBSearchUserCell.h
//  NAMEHEREMobile
//
//  Created by Kyler Mulherin on 1/14/15.
//  Copyright (c) 2015 NAMEHERE. All rights reserved.
//

#include <UIKit/UIKit.h>
#include "NAMEHEREImageView.h"
#include "NAMEHEREData.h"

@interface RBSearchUserCell : UICollectionViewCell

//cell visual elements
@property IBOutlet UILabel* lblName;
@property IBOutlet UITextView* lblBlurb;
@property IBOutlet UITextView* lblOtherNames;
@property IBOutlet UILabel* lblOtherNamesWord;
@property IBOutlet NAMEHEREImageView* imgAvatar;
@property IBOutlet UIImageView* imgIsOnline;

//data values
@property (strong, nonatomic) RBXUserSearchInfo* searchInfo;

+ (CGSize) getCellSize;
+ (NSString*) getNibName;

-(void) setInfo:(RBXUserSearchInfo *)searchInfo;
@end
