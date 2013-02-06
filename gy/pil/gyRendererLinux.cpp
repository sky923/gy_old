#include "gyRenderer.h"
#if defined(__GY_OS_ENABLED_LINUX__)
// For Linux
namespace gy
{
 	namespace r3
	{
		// X11연관 함수들
#define MAX_RENDERER_CONTEXT 8
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
			return current_renderer_id;
		}

		renderer_object_t getRendererObject(renderer_t rid)
		{
			return NullPtr;
		}

        renderer_t createRenderer(void *vid, size_t witdh, size_t height)
        {
            Display *display = view_context_t::display;
			if (display == NullPtr)
				return NullPtr;

            int screen = DefaultScreen(display);
            /* get an appropriate visual */
            XVisualInfo *vi = getVisualInfoFromView(vid);
            renderer_object_t rid = glXCreateContext(display, vi, 0, GL_TRUE);
            glXMakeCurrent(display, vid, rid);
            
			addRendererContext(rid, vid);

			int majorVersion = -1, minorVersion = -1;
            glGetIntegerv(GL_MAJOR_VERSION, (GLint *) (&majorVersion));
            glGetIntegerv(GL_MINOR_VERSION, (GLint *) (&minorVersion));
            printf("gl-Version %d.%d\n", majorVersion, minorVersion);
            
            return rid;
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
			Display *d = view_context_t::display;
			if (d == NullPtr)
				return Failed;

			view_t vid = getWindowFromRenderer(rid);
			glXSwapBuffers(d, vid);
			glFlush();

			return Success;
		}
	}
}
#endif
