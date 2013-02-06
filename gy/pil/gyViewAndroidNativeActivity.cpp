#include "gyViewAndroidNativeActivity.h"

#if defined(__GY_OS_ENABLED_ANDROID__)
#include <cassert>
namespace gy
{
	int suspended = 0;
	AAssetManager *asset_manager = NullPtr;
	ASensorManager *sensor_manager = NullPtr;
	const ASensor *sensor_accelerometer = NullPtr;
	const ASensor *sensor_gyroscope = NullPtr;
	ASensorEventQueue *sensor_event_queue = NullPtr;

	size_t width = 0;
	size_t height = 0;

	bool is_valid(view_t vid)
	{
		return vid && vid->activity && vid->activity->vm;
	}
}

void gy__onApplicationCommand(view_t vid, int32_t cmd)
{
	if (gy::is_valid(vid) == false)
	{
		assert(false);
		return;
	}

	switch (cmd)
	{
	case APP_CMD_SAVE_STATE:
		{
		} break;

	case APP_CMD_INIT_WINDOW:
		{
			// initialize renderer here
			gy::r3::create(vid);
			gy::r3::setClearColor(0xFF0000FF);
			// prepare (after creating a window)
			gy::type::default_callback_t applicationPrepare = gy::getCallback(gy::type::__cb::A_APPLICATION_PREPARE);
			if (applicationPrepare != NULL) applicationPrepare();
		} break;

	case APP_CMD_TERM_WINDOW:
		{
		} break;

	case APP_CMD_GAINED_FOCUS:
		{
			if (gy::sensor_accelerometer != NULL)
			{
				ASensorEventQueue_enableSensor(gy::sensor_event_queue, gy::sensor_accelerometer);
				ASensorEventQueue_setEventRate(gy::sensor_event_queue, gy::sensor_accelerometer, (int)((1000.0f / 60.0f) * 1000.0f));
			}
		} break;

	case APP_CMD_LOST_FOCUS:
		{
			if (gy::sensor_accelerometer != NULL)
			{
				ASensorEventQueue_disableSensor(gy::sensor_event_queue, gy::sensor_accelerometer);
			}

			gy::suspended = 0;
		} break;

	case APP_CMD_RESUME:
		{
		} break;

	case APP_CMD_PAUSE:
		{
		} break;
	}
}

int32_t gy__onInputEvent(view_t vid, AInputEvent *inputEvent)
{
	if (gy::is_valid(vid) == false)
		return 0;

	if (AInputEvent_getType(inputEvent) == AINPUT_EVENT_TYPE_MOTION)
	{
		gy::suspended = 1;
		return 1;
	}

	return 0;
}
#endif
