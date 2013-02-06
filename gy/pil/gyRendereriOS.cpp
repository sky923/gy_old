#include "gyRenderer.h"
#if defined(__GY_OS_ENABLED_IOS__)
// For iOS
namespace gy
{
	namespace r3
	{
		// X11연관 함수들
#define MAX_RENDERER_CONTEXT 1
		_renderer_object_t renderer_objects[MAX_RENDERER_CONTEXT] = {{NullPtr},};
		size_t current_renderer_id = GY_INVALID_RENDERER_ID;

		renderer_t getAllocableID()
		{
			for (size_t i = 0; i < MAX_RENDERER_CONTEXT; ++i)
			{
				if (isValid(i) == true)
					continue;

				return i;
			}

			return GY_INVALID_RENDERER_ID;
		}

		uint32 getMaxAllocableObjectCount()
		{
			return MAX_RENDERER_CONTEXT;
		}

		bool isValid()
		{
			return true;
		}

		bool isValid(renderer_t rid)
		{
			return true;
		}

		int setCurrentRendererID(renderer_t rid)
		{
			return Success;
		}

		renderer_t getCurrentID()
		{
			return Success;
		}

		renderer_object_t getRendererObject(renderer_t rid)
		{
			return NullPtr;
		}


		// OS별 특징이 있는 공통 함수들
		renderer_t createRenderer(void *vid, size_t witdh, size_t height)
		{
			return Success;
		}

		renderer_t createRenderer(const _renderer_object_t &rawRendererContext)
		{
			return Success;
		}

		int destroyRenderer(renderer_t rid)
		{
			return Success;
		}

		// OS별 특징이 없는 공통 함수들
		uint32 getClearColor()
		{
			return 0xFF0000FF;
		}

		int setClearColor(const uint32 &color)
		{
			return Success;
		}

		int clear()
		{
			return Success;
		}

		int begin()
		{
			return Success;
		}

		int end()
		{
			return Success;
		}

		int flush()
		{
			return Success;
		}
	}
}
#endif
