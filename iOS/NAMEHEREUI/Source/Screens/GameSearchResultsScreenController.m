//
//  GameSearchResultsScreenController.m
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 6/11/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import "GameSearchResultsScreenController.h"
#import "RBWebGamePreviewScreenController.h"
#import "GamesCollectionView.h"
#import "NAMEHERENotifications.h"
#import "NAMEHEREData.h"
#import "NAMEHERETheme.h"
#import "UIViewController+Helpers.h"
#import "Flurry.h"

//---METRICS---
#define GSRSC_showGameDetails   @"GAME SEARCH RESULTS SCREEN - Show Game Details"
#define GSRSC_showSearchResults @"GAME SEARCH RESULTS SCREEN - Show Search Results"

@interface GameSearchResultsScreenController ()

@end

@implementation GameSearchResultsScreenController
{
    GamesCollectionView* _collectionView;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

	self.view.backgroundColor = [NAMEHERETheme lightBackground];
    self.navigationItem.title = NSLocalizedString(@"SearchResultsPhrase", nil);
    
    self.edgesForExtendedLayout = UIRectEdgeNone;
    
    [self addSearchIconWithSearchType:SearchResultGames andFlurryEvent:nil];
    
    _collectionView = [[GamesCollectionView alloc] init];
    [self.view addSubview:_collectionView];
    
    [_collectionView loadGamesForKeywords:_keywords];
}

- (void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    
    _collectionView.frame = self.view.bounds;
}

@end
