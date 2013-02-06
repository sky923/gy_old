#include "gyViewAndroidNativeActivity.h"
#include "gyMainCore.h"
#if defined(__GY_OS_ENABLED_ANDROID__)
namespace gy
{
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
		main_view_id = vid;

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
		if (is_valid(vid) == false)
			return Failed;

		ANativeActivity_finish(vid->activity);

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
		struct android_app *vid = main_view_id;
		if (is_valid(vid) == false)
			return Failed;

		gy::r3::initialize(GY_RENDERER_OPENGLES);

		asset_manager = vid->activity->assetManager;
		sensor_manager = ASensorManager_getInstance();
		sensor_accelerometer = ASensorManager_getDefaultSensor(sensor_manager, ASENSOR_TYPE_ACCELEROMETER);
		sensor_gyroscope = ASensorManager_getDefaultSensor(sensor_manager, ASENSOR_TYPE_GYROSCOPE);
		sensor_event_queue = ASensorManager_createEventQueue(sensor_manager, vid->looper, LOOPER_ID_USER, NullPtr, NullPtr);
		vid->onAppCmd = gy__onApplicationCommand;
		vid->onInputEvent = gy__onInputEvent;

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
		struct android_app *vid = main_view_id;
		if (is_valid(vid) == false)
			return Failed;

		while (true)
		{
			int ident;
			int events;
			struct android_poll_source *source;

			// dispatch events from sensors
			while ((ident = ALooper_pollAll(!suspended ? 0 : -1, NULL, &events, (void **)&source)) >= 0)
			{
				if (source != NULL)
				{
					source->process(vid, source);
				}

				// process, exist data from sensors
				if (ident == LOOPER_ID_USER)
				{
					if (sensor_accelerometer != NULL)
					{
						ASensorEvent sensorEvent;
						while (ASensorEventQueue_getEvents(sensor_event_queue, &sensorEvent, 1) > 0)
						{
							//LOGI("accelerometer: x = %f, y = %f, z = %f", sensorEvent.acceleration.x, sensorEvent.acceleration.y, sensorEvent.acceleration.z);
						}
					}
				}

				// check of requesting destroy
				if (vid->destroyRequested != 0)
				{
					return Success;
				}
			}

			// application update and renderer update here
			if (!suspended)
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
		} // while (true)

		return Failed;
	}

	int defaultFinalizeApplication()
	{
		return Success;
	}
}
#endif