//
//  gyViewMacOSX.cpp
//  gy
//
//  Created by gil on 9/8/12.
//  Copyright (c) 2012 sky923. All rights reserved.
//

#import "gyViewMacOSXApplication.h"
#if defined(__GY_OS_ENABLED_MACOSX__)
namespace gy
{
	gy__NSApplication *gyApplication;
	NSAutoreleasePool *pool;
	view_t main_view_id = nil;
	
	view_t getParentView(view_t vid)
	{
		NSWindow *window = (NSWindow *)vid;
		if (window == nil)
			return NullPtr;
		
		return [window parentWindow];
	}
	
	int setParentView(view_t vid
						, view_t parentWindowID)
	{
		NSWindow *window = (NSWindow *)vid;
		if (window == nil)
			return Failed;
		
		NSWindow *parentWindow = (NSWindow *)parentWindowID;
		[window setParentWindow: parentWindow];
		
		if (parentWindow != nil)
		{
			[parentWindow addChildWindow: window
								 ordered: NSWindowBelow];
		}
		
		return Success;
	}
	
	view_t createView(const char *title
						  , size_t width
						  , size_t height
						  , int posX
						  , int posY)
	{
		if (pool == nil || gyApplication == nil)
			return NullPtr;
		
		NSScreen *mainScreen = [NSScreen mainScreen];
		NSRect appRect = [mainScreen frame];
		NSRect rect = NSMakeRect(posX == _Default ? 100 : posX
								 , posY == _Default ? appRect.size.height - 100 - height : appRect.size.height - posY - height
								 , width == _Default ? 320 : width
								 , height == _Default ? 240 : height);
		unsigned style = NSBorderlessWindowMask
			| NSTitledWindowMask
			| NSClosableWindowMask
			| NSResizableWindowMask
			| NSMiniaturizableWindowMask;
		
		id newWindow = [[NSWindow alloc] initWithContentRect: rect
												   styleMask: style
													 backing: NSBackingStoreBuffered
													   defer: NO];
		[newWindow setTitle: [NSString stringWithCString: title
												encoding: NSASCIIStringEncoding]];
		//[newWindow setReleasedWhenClosed: True];
		[newWindow setDelegate: gyApplication];
		[newWindow autorelease];
		
		setMainView(newWindow);
		
		return newWindow;
	}
	
	int showView(view_t vid)
	{
		NSWindow *window = (NSWindow *)vid;
		if (window == nil)
			return Failed;
		
		if (gyApplication == nil)
			return Failed;
		
		//[window makeKeyAndOrderFront: window];
		//[NSApp activateIgnoringOtherApps:YES];
		//Show Window
		[window orderFrontRegardless];
		
		return Success;
	}
	
	int hideView(view_t vid)
	{
		NSWindow *window = (NSWindow *)vid;
		if (window == nil)
			return Failed;
		
		[window orderOut: window];
		
		return Success;
	}
	
	int destroyView(view_t vid)
	{
		NSWindow *window = (NSWindow *)vid;
		if (window == nil)
			return Failed;
		
		[window close];
		
		return Success;
	}
	
	view_t getMainView()
	{
		return main_view_id;

		/*
		if (gyApplication == nil)
			return NullPtr;
		
		return [gyApplication mainWindow];
		 */
	}
	
	int setMainView(view_t vid)
	{
		main_view_id = vid;
		
		return Success;
		
		/*
		NSWindow *window = (NSWindow *)vid;
		if (window == nil)
			return Failed;

		[window makeMainWindow];
		[window makeKeyWindow];

		return Success;
		 */
	}
	
	int initializeDefaultView()
	{
		return Success;
	}

	int initializeDefaultApplication()
	{
		pool = [[NSAutoreleasePool alloc] init];
		
		gyApplication = (gy__NSApplication *)[gy__NSApplication sharedApplication];
		[gyApplication setActivationPolicy:NSApplicationActivationPolicyRegular];
		[gyApplication setDelegate:gyApplication];
		
		
		if (pool == nil || gyApplication == nil)
			return Failed;
		
		id menubar = [[NSMenu new] autorelease];
		id appMenuItem = [[NSMenuItem new] autorelease];
		[menubar addItem:appMenuItem];
		[NSApp setMainMenu:menubar];
		id appMenu = [[NSMenu new] autorelease];
		id appName = [[NSProcessInfo processInfo] processName];
		id quitTitle = [@"Quit " stringByAppendingString:appName];
		id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
													  action:@selector(terminate:)
											   keyEquivalent:@"q"] autorelease];
		[appMenu addItem:quitMenuItem];
		[appMenuItem setSubmenu:appMenu];
		[pool release];
		
		return Success;
	}
	
	int applicationRun()
	{
		if (pool == nil || gyApplication == nil)
			return Failed;
		
		[gyApplication run];
		[gyApplication autorelease];

		return Success;
	}
}

#endif
