#include "gyViewWindowsProcedure.h"
#include "gyMainCore.h"
#if defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
namespace gy
{
	view_t invalid_view_event = {NullPtr};
	view_t view_events[MAX_VIEW_STORE_SIZE] = {NullPtr,};
	size_t added_view_event_size = 0;
	bool isAlreadyExistView(view_t vid)
	{
		for (size_t i = 0; i < MAX_VIEW_STORE_SIZE; ++i)
		{
			view_t &v = view_events[i];
			if (v == vid)
				return true;
		}

		return false;
	}

	int storeView(view_t vid)
	{
		if (isAlreadyExistView(vid) == true)
			return Failed;

		for (size_t i = 0; i < MAX_VIEW_STORE_SIZE; ++i)
		{
			view_t &v = view_events[i];
			if (v != NullPtr)
				continue;

			v = vid;
			added_view_event_size++;

			return Success;
		}

		return Failed;
	}
	void clearViewEvent()
	{
		for (size_t i = 0; i < MAX_VIEW_STORE_SIZE; ++i)
		{
			view_events[i] = NullPtr;
		}

		added_view_event_size = 0;
	}

	void clearViewEvent(view_t vid)
	{
		for (size_t i = 0; i < MAX_VIEW_STORE_SIZE; ++i)
		{
			view_t &v = view_events[i];
			if (v != vid)
				continue;

			v = NullPtr;

			added_view_event_size--;
			printf("added_view_event_size = %d\n", added_view_event_size);
			return;
		}
	}

	size_t getViewEventSize()
	{
		return added_view_event_size;
	}

	bool isParentView(view_t beginViewID, view_t findingViewID)
	{
		if (beginViewID == NullPtr || findingViewID == NullPtr)
			return false;

		// 찾고자하는 windowID(findingWindowID)를 beginWindowID로부터 역순회하며 부모 windowID들과 비교한다.
		view_t itr = beginViewID;
		while (itr != NullPtr)
		{
			// 찾고자하는 windowID(findingWindowID)가 존재하면, 그것(findingWindowID)은 부모 window가 아니다.
			if (itr == findingViewID)
			{
				return false;
			}

			itr = GetParent(itr);
		}

		itr = findingViewID;
		while (itr != NullPtr)
		{
			if (itr == beginViewID)
			{
				return true;
			}

			itr = GetParent(itr);
		}

		return false;
	}
}

LRESULT CALLBACK __WndProc__(view_t vid, uint msg, WPARAM wParam, LPARAM lParam)
{
	static bool isActivated = false;
	switch (msg)
	{
	case WM_NCCREATE:
		{
			// initialize renderer here
			RECT windowRect;
			GetWindowRect(vid, &windowRect);
			gy::r3::createRenderer(vid, windowRect.right, windowRect.bottom);
			gy::r3::setClearColor(0xFF0000FF);
			// prepare (after creating a window)
			gy::type::default_callback_t applicationPrepare = gy::getCallback(gy::type::__cb::A_APPLICATION_PREPARE);
			if (applicationPrepare != NULL) applicationPrepare();
		} break;
	case WM_ACTIVATE:
		isActivated = static_cast<bool>(wParam != 0);
		break;
	case WM_INPUT:
		{
			if (isActivated == false)
				return 0;

			UINT dwSize = 0;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			if (dwSize <= 0)
				return 0;

			BYTE *lpb = new BYTE[dwSize];
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
			RAWINPUT *raw = (RAWINPUT *)lpb;
			if (raw->header.dwType == RIM_TYPEKEYBOARD) 
			{
				/*
				char szTempOutput[256] = {0};
				sprintf(szTempOutput, " Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04d \n", 
				raw->data.keyboard.MakeCode, 
				raw->data.keyboard.Flags, 
				raw->data.keyboard.Reserved, 
				raw->data.keyboard.ExtraInformation, 
				raw->data.keyboard.Message, 
				raw->data.keyboard.VKey);
				printf(szTempOutput);
				*/
				//for testing
				if (raw->data.keyboard.VKey == VK_ESCAPE)
					gy::clearViewEvent();

				//@todo: tanslation keyboard event to gy style in here!
				//gy::translateKeyboardEvent();
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE) 
			{
				/*
				char szTempOutput[256] = {0};
				sprintf(szTempOutput, "Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n", 
				raw->data.mouse.usFlags, 
				raw->data.mouse.ulButtons, 
				raw->data.mouse.usButtonFlags, 
				raw->data.mouse.usButtonData, 
				raw->data.mouse.ulRawButtons, 
				raw->data.mouse.lLastX, 
				raw->data.mouse.lLastY, 
				raw->data.mouse.ulExtraInformation);
				printf(szTempOutput);
				*/
				//@todo: translation mouse(or touch) event to gy style in here!
				//gy::trnaslateMouseEvent();
			} 

			delete []lpb;
		} return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_QUIT:
		DestroyWindow(vid);
		return 0;
	case WM_DESTROY:
		view_t mainViewID = gy::getMainView();
		if (vid == mainViewID || gy::isParentView(vid, mainViewID))
		{
			printf("0x%08x\n", vid);
			gy::clearViewEvent();
		}
		else
		{
			gy::clearViewEvent(vid);
		}
		return 0;
	}

	return DefWindowProc(vid, msg, wParam, lParam);
}
#endif