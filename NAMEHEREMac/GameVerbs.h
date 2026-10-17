/*
 *  GameVerbs.h
 *  MacClient
 *
 *  Created by Tony on 2/2/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "v8tree/Verb.h"
using namespace RBX;

class LeaveGameVerb : public RBX::Verb
{
	class NAMEHEREView *namehereView;
	
public :
	
	
	LeaveGameVerb(class NAMEHEREView *pNAMEHEREView, VerbContainer* container);

	virtual void doIt(RBX::IDataState* dataState);
	
};

class ShutdownClientVerb : public RBX::Verb
{
	class NAMEHEREView *namehereView;
	
public :
	
	ShutdownClientVerb( class NAMEHEREView *pNAMEHEREView, VerbContainer* container );
	
	virtual void doIt(RBX::IDataState* dataState);
};

class ToggleFullscreenVerb : public RBX::Verb
{
	
private:
	class VideoControl* videoControl;
	class NAMEHEREView *namehereView;
	
public:
	
	ToggleFullscreenVerb(class NAMEHEREView *pNAMEHEREView, VerbContainer* container, VideoControl* videoControl);
	virtual void doIt(RBX::IDataState* dataState);
	virtual bool isEnabled() const;
};