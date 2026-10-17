//
//  NAMEHEREMemoryManager.h
//  NAMEHEREMobile
//
//  Created by Martin Robaszewski on 5/15/13.
//  Copyright (c) 2013 NAMEHERE. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "NAMEHEREWebUtility.h"
#import "iOSSettingsService.h"

vm_size_t usedMemory(void);
vm_size_t freeMemory(void);
void print_free_memory();
void debug_stealMemory(size_t bytes);

@interface NAMEHEREMemoryManager : NSObject
{
    // memory bouncer - iOSClientSettings
    BOOL isMemoryBouncerActive;
    int memoryBouncerEnforceRate;
    int memoryBouncerThresholdKB;
    int memoryBouncerLimitMB;
    int memoryBouncerBlockSizeKB;
    BOOL bMemBouncerCrashed;
    
    // memory bouncer
    void * balloonMemoryBlock;
    std::vector<void *> memBlocks;
    NSTimer * memoryBouncerTimer;
    int stopCount;
    
    // free memory checking
    NSTimer * freeMemoryCheckerTimer;
    BOOL isFreeMemoryCheckerActive;
    int freeMemoryCheckerRate; // in seconds
    int freeMemoryCheckerTheshold; // in KiloBytes
    
    // max memory tracker
    NSTimer * maxMemoryTrackerTimer;
    int maxMemoryTrackerRate;
    NSInteger maxMemoryUsed;
}

+ (id)sharedInstance;

// memory bouncer functions
-(void) startMemoryBouncer;
-(BOOL) stopMemoryBouncer:(BOOL)bForce;
-(void) balloonMemory;
-(void) popBalloon;
-(void) bounceFreeMemory:(NSTimer *)timer;
-(void) memBouncerCrashed;

// free memory checking
-(void) startFreeMemoryChecker;
-(void) stopFreeMemoryChecker;
-(void) checkFreeMemory:(NSTimer *)timer;
-(void) logMemUsage:(NSTimer *)timer;

// max memory reporting
-(void) startMaxMemoryTracker;
-(void) checkMaxMemory:(NSTimer *)timer;
-(void) stopMaxMemoryTracker;

@end
