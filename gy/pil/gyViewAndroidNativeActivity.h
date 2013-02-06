#ifndef __gy_pil_gyViewAndroidNativeActivity_h__
#define __gy_pil_gyViewAndroidNativeActivity_h__

#include "gyRenderer.h"
#include "gyView.h"
#if defined(__GY_OS_ENABLED_ANDROID__)
#include <android/sensor.h>
#include <android/log.h>
namespace gy
{
	extern int suspended;
	extern AAssetManager *asset_manager;
	extern ASensorManager *sensor_manager;
	extern const ASensor *sensor_accelerometer;
	extern const ASensor *sensor_gyroscope;
	extern ASensorEventQueue *sensor_event_queue;

	extern size_t width;
	extern size_t height;

	bool is_valid(view_t vid);
}

void gy__onApplicationCommand(view_t vid, int32_t cmd);
int32_t gy__onInputEvent(view_t vid, AInputEvent *inputEvent);

#endif

#endif