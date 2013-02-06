#ifndef __gy_pil_gyViewWindowsProcedure_h__
#define __gy_pil_gyViewWindowsProcedure_h__

#include "gyRenderer.h"
#include "gyView.h"
#if defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
#define MAX_VIEW_STORE_SIZE 256
namespace gy
{
	int storeView(HWND vid);
	size_t getViewEventSize();
}

LRESULT CALLBACK __WndProc__(HWND vid, uint msg, WPARAM wParam, LPARAM lParam);

#endif

#endif