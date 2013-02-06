//
//  Header.h
//  gy
//
//  Created by gil on 9/8/12.
//  Copyright (c) 2012 sky923. All rights reserved.
//

#ifndef __gy_pil_gyView_h__
#define __gy_pil_gyView_h__

#include "gyAtomicDataType.h"

namespace gy
{
	namespace type
	{
#if defined(__GY_OS_ENABLED_ANDROID__)
#include <android_native_app_glue.h>
		typedef struct android_app *view_t;
#elif defined(__GY_OS_ENABLED_IOS__)
		typedef void *view_t;
#elif defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
#   include <windows.h>
		typedef HWND view_t;
#elif defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
		typedef Windows::ApplicationModel::Core::IFrameworkView ^view_t;
#elif defined(__GY_OS_ENABLED_MACOSX__)
		typedef void *view_t;
#elif defined(__GY_OS_ENABLED_LINUX__)
#   include <X11/Xlib.h>
#   include <X11/Xutil.h>
#   include <X11/Xos.h>
#   include <X11/Xatom.h>
#   include <X11/keysym.h>
#   include <X11/extensions/xf86vmode.h>
#   include <sys/select.h>
#   include <sys/types.h>
#   include <sys/time.h>
		typedef Window view_t;
        struct view_context_t
        {
            static Display *display;
            view_t view;
            XVisualInfo *visual;
            XSetWindowAttributes attributes;
            int screen;
            int posX;
            int posY;
            size_t width;
            size_t height;
        };
#else
		typedef void *view_t;
#error Unsupported Platform!
#endif
	}
	using type::view_t;
	
    // OS특화 도움 함수들 선언
#if defined(__GY_OS_ENABLED_ANDROID__)
	extern view_t main_view_id;
#elif defined(__GY_OS_ENABLED_IOS__)
#elif defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
#elif defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
#elif defined(__GY_OS_ENABLED_MACOSX__)
#elif defined(__GY_OS_ENABLED_LINUX__)
    using type::view_context_t;

    // X11연관 함수들
    XVisualInfo *getVisualInfoFromView(view_t vid);
#endif

	view_t createView(const char *title
					 , size_t width = _Default
					 , size_t height = _Default
					 , int posX = _Default
					 , int posY = _Default);

	int connectView(view_t vid);
	
	int showView(view_t vid);
	
	int hideView(view_t vid);
	
	int destroyView(view_t vid);
	
	view_t getMainView();
	
	int setMainView(view_t vid);
	
	int defaultInitializeView();

	int defaultInitializeApplication();
	
	int defaultRendererUpdate();

	int defaultApplicationUpdate();

	int defaultExecuteApplication();

	int defaultFinalizeApplication();
}
using gy::type::view_t;

#endif
