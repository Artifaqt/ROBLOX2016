//
//  NAMEHEREView.mm
//  MacClient
//
//  Created by Tony on 2/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ApplicationServices/ApplicationServices.h>

#import "NAMEHEREOgreView.h"

#include "NAMEHEREView.h"
#include "FunctionMarshaller.h"
#include "NAMEHEREPlayerAppDelegate.h"

inline void flipY(NSView *view, NSPoint &pt)
{
	BOOL flipped = [view isFlipped];
	
	if (!flipped)
	{
		NSRect bounds = [view bounds];
		CGFloat height = bounds.size.height;
		
		pt.y = height - pt.y;
	}	
}

void NAMEHEREView::getCursorPos(G3D::Vector2 *pPos)
{
    if(NAMEHEREOgreView* ogreView = (NAMEHEREOgreView*) viewWindow)
    {
        CGPoint virtualCursorPos = [ogreView getVirtualCursorPos];
        pPos->x = virtualCursorPos.x;
        pPos->y = virtualCursorPos.y;
    }
}

void NAMEHEREView::setCursorPos(G3D::Vector2 pPos)
{
    if(NAMEHEREOgreView* ogreView = (NAMEHEREOgreView*) viewWindow)
        [ogreView setVirtualCursorPos:CGPointMake(G3D::iRound(pPos.x), G3D::iRound(pPos.y))];
}

void NAMEHEREView::marshalTeleport(std::string url, std::string ticket, std::string script)
{
	marshaller->Submit(boost::bind(&NAMEHEREView::doTeleport, this, url, ticket, script));
}

void NAMEHEREView::doTeleport(std::string url, std::string ticket, std::string script)
{
	NAMEHEREPlayerAppDelegate *appDelegate = (NAMEHEREPlayerAppDelegate*)appWindow;

	NSString *ticketString = [NSString stringWithCString:ticket.c_str() encoding:[NSString defaultCStringEncoding]];
	NSString *urlString = [NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
	NSString *scriptString = [NSString stringWithCString:script.c_str() encoding:[NSString defaultCStringEncoding]];
	
	[appDelegate teleport:ticketString withAuthentication:urlString withScript:scriptString];
}

bool NAMEHEREView::isFullscreenMode()
{
    if(NAMEHEREOgreView* ogreView = (NAMEHEREOgreView*) viewWindow)
        return ogreView.fullScreen;
    
    return false;
}