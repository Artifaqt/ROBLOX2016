/*
 *  GameVerbs.cpp
 *  MacClient
 *
 *  Created by Tony on 2/2/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameVerbs.h"
#include "NAMEHEREView.h"

LeaveGameVerb::LeaveGameVerb(NAMEHEREView *pNAMEHEREView, VerbContainer* container) : 
	Verb(container, "Exit")
	,namehereView(pNAMEHEREView)
{
}

void LeaveGameVerb::doIt(RBX::IDataState* dataState)
{
	if (namehereView)
	{
		namehereView->leaveGame();
	}
}

ShutdownClientVerb::ShutdownClientVerb(NAMEHEREView *pNAMEHEREView, VerbContainer* container) :
	Verb( container, "ShutdownClient")
	,namehereView(pNAMEHEREView)
{
}

void ShutdownClientVerb::doIt(RBX::IDataState* dataState)
{
	if (namehereView)
	{
		namehereView->shutdownClient();
	}
}

ToggleFullscreenVerb::ToggleFullscreenVerb(NAMEHEREView *pNAMEHEREView, VerbContainer* container, VideoControl* videoControl) :
	Verb(container, "ToggleFullScreen")
	, videoControl(videoControl)
	,namehereView(pNAMEHEREView)
{}

void ToggleFullscreenVerb::doIt(RBX::IDataState* dataState)
{
	if (namehereView)
	{
		namehereView->toggleFullScreen();
	}
}

bool ToggleFullscreenVerb::isEnabled() const
{
	return true; 
}
