//
//  gyViewMacOSXApplication.h
//  gy
//
//  Created by gil on 9/8/12.
//  Copyright (c) 2012 sky923. All rights reserved.
//

#ifndef __gy_gyViewMacOSXApplication__
#define __gy_gyViewMacOSXApplication__

#include "gyRenderer.h"
#include "gyView.h"
#if defined(__GY_OS_ENABLED_MACOSX__)
// For Cocoa
#import <Cocoa/Cocoa.h>

@interface gy__NSApplication : NSApplication <NSApplicationDelegate, NSWindowDelegate>
{
}

-(void) windowDidBecomeKey:(NSNotification *)aNotification;
-(void) windowDidResignKey:(NSNotification *)aNotification;
-(void) windowShouldClose:(id)sender;
-(void) windowWillClose:(NSNotification *)aNotification;
-(void) windowDidResize:(NSNotification *)aNotification;
-(void) applicationDidFinishLaunching:(NSNotification *)aNotification;
-(BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender;
-(void) sendEvent:(NSEvent *)theEvent;
-(void) run;
-(void) terminate:(id)sender;
@end

#endif

#endif /* defined(__gy__gyViewMacOSXApplication__) */

