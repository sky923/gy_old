//
//  gyViewMacOSXApplication.cpp
//  gy
//
//  Created by gil on 9/8/12.
//  Copyright (c) 2012 sky923. All rights reserved.
//

#include "gyViewMacOSXApplication.h"
#include "gyView.h"
#include "gyMainCore.h"

#if defined(__GY_OS_ENABLED_MACOSX__)

@implementation gy__NSApplication
-(void) windowDidBecomeKey:(NSNotification *)aNotification
{
}
-(void) windowDidResignKey:(NSNotification *)aNotification
{
}
-(void) windowShouldClose:(id)sender
{
	if (sender == (__bridge id)(gy::getMainView()))
		[self terminate: self];
}
-(void) windowWillClose:(NSNotification *)aNotification
{
}
-(void) windowDidResize:(NSNotification *)aNotification
{
}
-(void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
}
-(BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
	return YES;
}

-(void) sendEvent:(NSEvent *)theEvent
{
	NSWindow *window = [theEvent window];
	ulong eventType = [theEvent type];
	if (eventType == NSLeftMouseDown)
	{
		//[window isMainWindow];
		if (gy::getMainView() == (__bridge view_t)(window))
		{
		}
		
		//_running = NO; //for testing (임시)
		// 이곳에 지정된 메시지만을 처리한다.
		// 지정되지 않은 경우, super에 sendEvent메시지를 보낸다.
		//return;
    }
	
	[super sendEvent:theEvent];
}

-(void) run
{
	[self finishLaunching];
    _running = YES;
    
    // prepare (after creating a window)
    gy::type::default_callback_t applicationPrepare = gy::getCallback(gy::type::__cb::A_APPLICATION_PREPARE);
    if (applicationPrepare != NULL) applicationPrepare();
	
    NSEvent* event;
    do
    {
        event = [self nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
		
        if (event == nil)
		{
            // LoopFunc 필요한 함수를 호출하세요.
            gy::type::default_callback_t applicationUpdate = gy::getCallback(gy::type::__cb::A_APPLICATION_UPDATER);
            if (applicationUpdate != NULL)
            {
                if (applicationUpdate() == Failed)
                    return;
            }
            else
            {
                assert(false);
                return;
            }
        }
		else
		{
            [self sendEvent:event];
        }
        
        gy::type::default_callback_t rendererUpdate = gy::getCallback(gy::type::__cb::A_RENDERER_UPDATER);
        if (rendererUpdate != NULL) rendererUpdate();
		
    } while ([self isRunning]);
}

-(void) terminate:(id)sender
{
	_running = NO;
    
    gy::defaultFinalizeApplication();
}

@end

#endif