/*
 *  GameVerbs.mm
 *  iOS
 *
 *  Created on 5/19/14
 *  Copyright 2014 NAMEHERE. All rights reserved.
 *
 */

#import "PlaceLauncher.h"

#include "GameVerbs.h"
#include "NAMEHEREView.h"

#include "v8datamodel/GameBasicSettings.h"

LeaveGameVerb::LeaveGameVerb(NAMEHEREView *pNAMEHEREView, VerbContainer* container) : 
	Verb(container, "Exit")
	,namehereView(pNAMEHEREView)
{
}

void LeaveGameVerb::doIt(RBX::IDataState* dataState)
{
	[[PlaceLauncher sharedInstance] leaveGame:YES];
    
    RBX::GlobalBasicSettings::singleton()->saveState();
}
