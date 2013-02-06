#include "gyViewWindowsStoreApplication.h"
#include "gyView.h"
#if defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
namespace gy
{
	gy__ApplicationSource ^appSource = NullPtr;
	view_t main_view_id = NullPtr;

	/// 안드로이드의 경우, View의 이름, 위치와 크기가 무시된다.
	view_t createView(const char *title
					 , size_t width
					 , size_t height
					 , int posX
					 , int posY)
	{
		return main_view_id;
	}

	int connectView(view_t vid)
	{
		return Success;
	}
	
	int showView(view_t vid)
	{
		return Success;
	}
	
	int hideView(view_t vid)
	{
		return Success;
	}
	
	int destroyView(view_t vid)
	{
		if (vid == NullPtr)
			return Failed;

		vid->Uninitialize();
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
	
	int defaultInitializeView()
	{
		return Success;
	}

	int defaultInitializeApplication()
	{
		gy::r3::initializeRenderer(GY_RENDERER_OPENGLES);

		appSource = ref new gy__ApplicationSource();

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
		if (appSource == NullPtr)
			return Failed;

		CoreApplication::Run(appSource);

		return Success;
	}

	int defaultFinalizeApplication()
	{
		return Success;
	}
}
#endif