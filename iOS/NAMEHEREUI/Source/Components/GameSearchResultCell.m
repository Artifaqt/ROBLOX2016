//
//  GameSearchResultCell.m
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 6/10/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import "GameSearchResultCell.h"
#import "NAMEHEREImageView.h"
#import "NAMEHERETheme.h"
#import "NAMEHEREInfo.h"
#import "RBActivityIndicatorView.h"

@implementation GameSearchResultCell
{
    IBOutlet NAMEHEREImageView* _thumbnail;
    IBOutlet UILabel *_title;
    IBOutlet RBActivityIndicatorView *_activityIndicator;
}

+(CGSize)getCellSize
{
    return [NAMEHEREInfo thisDeviceIsATablet] ? CGSizeMake(160, 220) : CGSizeMake(140, 215);
}
+(NSString*)getNibName
{
    return [NAMEHEREInfo thisDeviceIsATablet] ? @"GameSearchResultCell" : @"GameSearchResultCell_iPhone";
}

- (void)awakeFromNib
{
	[NAMEHERETheme applyToGameSearchToolbarCell:_title];
    _thumbnail.animateInOptions = RBXImageViewAnimateInIfNotCached;
    _thumbnail.layer.cornerRadius = 5;
    _thumbnail.layer.masksToBounds = YES;
    _thumbnail.layer.borderColor = [UIColor whiteColor].CGColor;
    _thumbnail.layer.borderWidth = 1;
    
    _title.hidden = YES;
    _activityIndicator.hidden = NO;
}

- (void)setGameData:(RBXGameData *)gameData
{
    if (gameData)
    {
        _gameData = gameData;
        _title.text = gameData.title;
        _title.hidden = NO;
        
        [_activityIndicator startAnimating];
        [_thumbnail loadThumbnailForGame:gameData withSize:[NAMEHERETheme sizeGameCoverSquare] completion:^{
            dispatch_async(dispatch_get_main_queue(), ^
           {
               [_activityIndicator stopAnimating];
               _thumbnail.hidden = NO;
           });
        }];
    }
    else
    {
        _title.hidden = YES;
        _thumbnail.hidden = YES;
        [_activityIndicator startAnimating];
    }
    
}

@end
