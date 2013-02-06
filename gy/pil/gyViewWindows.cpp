#include "gyViewWindowsProcedure.h"
#include "gyMainCore.h"

#if defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_KEYBOARD
#define HID_USAGE_GENERIC_KEYBOARD        ((USHORT) 0x06)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

namespace gy
{
	view_t main_view_id = NullPtr;

	view_t getParentView(view_t vid)
	{
		return GetParent(vid);
	}

	int setParentView(view_t vid, view_t parentViewID)
	{
		if (vid == NullPtr)
			return Failed;

		view_t mainViewID = getMainView();
		if (parentViewID != NullPtr && mainViewID == vid)
			setMainView(parentViewID);

		SetParent(vid, parentViewID);

		return Success;
	}

	view_t getMainView()
	{
		return main_view_id;
	}

	int setMainView(view_t vid)
	{
		main_view_id = vid;

		return Success;
	}

	// 공통 함수들
	view_t createView(const char *_title, size_t width, size_t height, int posX, int posY)
	{
#ifndef _UNICODE
		const char *title = _title;
#else
		size_t titleLen = strlen(_title);
		wchar_t *title = new wchar_t[titleLen + 1];
		mbstowcs_s(&titleLen, title, titleLen + 1, _title, titleLen + 1);
#endif

		WNDCLASS wndClass;

		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wndClass.hCursor = LoadCursor(NullPtr, IDC_ARROW);
		wndClass.hIcon = LoadIcon(NullPtr, IDI_APPLICATION);
		wndClass.hInstance = GetModuleHandle(NullPtr);
		wndClass.lpfnWndProc = (WNDPROC)__WndProc__;
		wndClass.lpszClassName = title;
		wndClass.lpszMenuName = NullPtr;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;// | CS_OWNDC;
		RegisterClass(&wndClass);
		view_t vid = CreateWindow(title, title, WS_OVERLAPPEDWINDOW,
			posX == _Default ? CW_USEDEFAULT : posX,
			posY == _Default ? CW_USEDEFAULT : posY,
			width == _Default ? CW_USEDEFAULT : width,
			height == _Default ? CW_USEDEFAULT : height,
			NullPtr, (HMENU)0, GetModuleHandle(NullPtr), NullPtr);


		setMainView(vid);
		storeView(vid);

		RAWINPUTDEVICE rawInputDev[2];
		ZeroMemory(rawInputDev, sizeof(RAWINPUTDEVICE) * 2);

		// 키보드 RAWINPUTDEVICE 구조체 설정
		rawInputDev[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		rawInputDev[0].usUsage = HID_USAGE_GENERIC_KEYBOARD;
		rawInputDev[0].dwFlags = RIDEV_INPUTSINK;
		rawInputDev[0].hwndTarget = vid;

		//마우스 RAWINPUTDEVICE 구조체 설정
		rawInputDev[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
		rawInputDev[1].usUsage = HID_USAGE_GENERIC_MOUSE;
		rawInputDev[1].dwFlags = RIDEV_INPUTSINK;
		rawInputDev[1].hwndTarget = vid;  

		if(RegisterRawInputDevices(rawInputDev, 2, sizeof(RAWINPUTDEVICE)) == False)
		{
			printf("Failed initialization of raw input devices.\n");
		} 

		return vid;
	}

	int showView(view_t vid)
	{
		if (ShowWindow(vid, SW_SHOW))
			return Success;

		return Failed;
	}

	int hideView(view_t vid)
	{
		if (ShowWindow(vid, SW_HIDE))
			return Success;

		return Failed;
	}

	int destroyView(view_t vid)
	{
		if (vid == NullPtr)
			return Failed;

		DestroyWindow(vid);

		return Success;
	}

	int defaultInitializeView()
	{
		view_t vid = gy::createView("gy", 1280, 720);
		gy::showView(vid);
		return Success;
	}

	int defaultInitializeApplication()
	{
		gy::r3::initializeRenderer(GY_RENDERER_OPENGL);

		return Success;
	}

	int defaultRendererUpdate()
	{
		gy::r3::clear();
		gy::r3::begin();
		gy::r3::end();
		gy::r3::flush();
		return Success;
	}

	int defaultApplicationUpdate()
	{
		return Success;
	}

	int defaultExecuteApplication()
	{
		MSG msg;
		while (true)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				type::default_callback_t applicationUpdate = gy::getCallback(gy::type::__cb::A_APPLICATION_UPDATER);
				if (applicationUpdate != NULL) 
				{
					if (applicationUpdate() == Failed)
						return Failed;
				}
				else
				{
					assert(false);
					return Failed;
				}

				type::default_callback_t rendererUpdate = gy::getCallback(gy::type::__cb::A_RENDERER_UPDATER);
				if (rendererUpdate != NULL) rendererUpdate();
			}

			if (getViewEventSize() == 0)
			{
				return Success;
			}
		}

		return Failed;
	}

	int defaultFinalizeApplication()
	{
		return Success;
	}
}

#endif