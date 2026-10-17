//
//  NAMEHERE.m
//  NAMEHEREMobile
//
//  Created by Ben Tkacheff on 1/30/13.
//  Copyright (c) 2013 NAMEHERE. All rights reserved.
//

#import "MarshallerInterface.h"
#include "FunctionMarshaller.h"
#include "NAMEHERE.h"


@implementation FunctionMarshaller

@synthesize pClosure;

-(void) marshallFunction
{
    RBX::FunctionMarshaller::handleAppEvent(pClosure);
}

@end




static NSString* kAppEventMode = @"NAMEHEREAppEvent";
static NSArray* kAppEventModeList = [NSArray arrayWithObjects:(NSString*)kCFRunLoopDefaultMode, kAppEventMode, nil];

void NAMEHERE::sendAppEvent(void *pClosure)
{
    RBX::CEvent *waitEvent = ((RBX::FunctionMarshaller::Closure *) pClosure)->waitEvent;
	BOOL waitFlag = (waitEvent == NULL);
    
    FunctionMarshaller* controller = [[FunctionMarshaller alloc] init];
    controller->pClosure = pClosure;
    
    [controller performSelectorOnMainThread:@selector(marshallFunction) withObject:nil waitUntilDone:waitFlag modes:kAppEventModeList];
	
	if (waitEvent)
	{
		waitEvent->Wait();
	}
}

void NAMEHERE::postAppEvent(void *pClosure)
{
    FunctionMarshaller* controller = [[FunctionMarshaller alloc] init];
    controller->pClosure = pClosure;
    
    [controller performSelectorOnMainThread:@selector(marshallFunction) withObject:nil waitUntilDone:NO modes:kAppEventModeList];
}

void NAMEHERE::processAppEvents()
{
    while (CFRunLoopRunInMode((CFStringRef)kAppEventMode, 0, true) == kCFRunLoopRunHandledSource)
        ;
}