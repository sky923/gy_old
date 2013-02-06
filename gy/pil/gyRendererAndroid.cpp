#include "gyRenderer.h"
#if defined(__GY_OS_ENABLED_ANDROID__)
// For Android
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;
namespace gy
{
	namespace r3
	{
		// X11연관 함수들
#define MAX_RENDERER_CONTEXT 1
		_renderer_object_t renderer_objects[MAX_RENDERER_CONTEXT] = {{EGL_NO_DISPLAY, EGL_NO_CONTEXT, EGL_NO_SURFACE, 0},};
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
			const renderer_t rid = getCurrentID();
			return isValid(rid);
		}

		bool isValid(const renderer_t rid)
		{
			if (rid == GY_INVALID_RENDERER_ID)
				return false;

			int rendererAPIType = getRendererAPIType();
			renderer_object_t robj = &renderer_objects[rid];
			bool ret = rendererAPIType != GY_RENDERER_UNKNOWN 
				&& rendererAPIType < GY_RENDERER_TYPE_SIZE 
				&& robj != NullPtr
				&& robj->Window != NULL
				&& robj->Display != NULL
				&& robj->Surface != NULL
				&& robj->Context != NULL;
				//&& robj->RendererContext != wglGetCurrentContext();

			return ret;
		}

		int setCurrentRendererID(renderer_t rid)
		{
			if (rid == GY_INVALID_RENDERER_ID)
			{
				return Failed;
			}

			current_renderer_id = rid;
			renderer_object_t robj = &renderer_objects[rid];
			if (isValid(rid) == false)
			{
				return Failed;
			}

			int rendererAPIType = getRendererAPIType();
			if (rendererAPIType == GY_RENDERER_OPENGLES)
			{
				if (eglMakeCurrent(robj->Display, robj->Surface, robj->Surface, robj->Context) != EGL_TRUE)
					return Failed;
			}

			return Success;
		}

		renderer_t getCurrentID()
		{
			return current_renderer_id;
		}

		renderer_object_t getRendererObject(renderer_t rid)
		{
			if (isValid(rid) == false)
				return NullPtr;

			return &renderer_objects[rid];
		}


		// OS별 특징이 있는 공통 함수들
		renderer_t createRenderer(void *vid, size_t witdh, size_t height)
		{
			const renderer_t allocableID = getAllocableID();
			if (allocableID == GY_INVALID_RENDERER_ID)
			{
				assert(false);
				return GY_INVALID_RENDERER_ID;
			}

			renderer_object_t robj = &renderer_objects[allocableID];
			robj->Window = (ANativeWindow *)((struct android_app *)vid)->window;
			const int samples = 2; // for supporting multisampling
			EGLint eglConfigAttrs[] =
			{
				EGL_SAMPLE_BUFFERS,     samples > 0 ? 1 : 0,
				EGL_SAMPLES,            samples,
				EGL_DEPTH_SIZE,         24,
				EGL_RED_SIZE,           8,
				EGL_GREEN_SIZE,         8,
				EGL_BLUE_SIZE,          8,
				EGL_ALPHA_SIZE,         8,
				EGL_STENCIL_SIZE,       8,
				EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
				EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
				EGL_NONE
			};

			EGLint eglConfigCount;
			const EGLint eglContextAttrs[] =
			{
				EGL_CONTEXT_CLIENT_VERSION,    2,
				EGL_NONE
			};

			const EGLint eglSurfaceAttrs[] =
			{
				EGL_RENDER_BUFFER,    EGL_BACK_BUFFER,
				EGL_NONE
			};

			robj->Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			if (robj->Display == EGL_NO_DISPLAY)
			{
				//checkErrorEGL("eglGetDisplay");
				//goto error;
				return GY_INVALID_RENDERER_ID;
			}

			if (eglInitialize(robj->Display, NULL, NULL) != EGL_TRUE)
			{
				return GY_INVALID_RENDERER_ID;
			}

			// Try both 24 and 16-bit depth sizes since some hardware (i.e. Tegra) does not support 24-bit depth
			bool validConfig = false;
			EGLint depthSizes[] = {24, 16};
			for (unsigned int i = 0; i < 2; ++i)
			{
				eglConfigAttrs[1] = samples > 0 ? 1 : 0;
				eglConfigAttrs[3] = samples;
				eglConfigAttrs[5] = depthSizes[i];

				if (eglChooseConfig(robj->Display, eglConfigAttrs, &robj->Config, 1, &eglConfigCount) == EGL_TRUE && eglConfigCount > 0)
				{
					validConfig = true;
					break;
				}

				if (samples)
				{
					// Try lowering the MSAA sample size until we find a supported config
					int sampleCount = samples;
					while (sampleCount)
					{
						//GP_WARN("No EGL config found for depth_size=%d and samples=%d. Trying samples=%d instead.", depthSizes[i], sampleCount, sampleCount / 2);
						sampleCount /= 2;//sampleCount >> 1;
						eglConfigAttrs[1] = sampleCount > 0 ? 1 : 0;
						eglConfigAttrs[3] = sampleCount;
						if (eglChooseConfig(robj->Display, eglConfigAttrs, &robj->Config, 1, &eglConfigCount) == EGL_TRUE && eglConfigCount > 0)
						{
							validConfig = true;
							break;
						}
					}
					if (validConfig)
						break;
				}
				else
				{
					//GP_WARN("No EGL config found for depth_size=%d.", depthSizes[i]);
				}
			}

			if (validConfig == false)
			{
				assert(false);
				return GY_INVALID_RENDERER_ID;
			}

			robj->Context = eglCreateContext(robj->Display, robj->Config, EGL_NO_CONTEXT, eglContextAttrs);
			if (robj->Context == EGL_NO_CONTEXT)
			{
				assert(false);
				return GY_INVALID_RENDERER_ID;
			}

			// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
			// guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
			// As soon as we picked a EGLConfig, we can safely reconfigure the
			// ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
			EGLint format;
			eglGetConfigAttrib(robj->Display, robj->Config, EGL_NATIVE_VISUAL_ID, &format);
			ANativeWindow_setBuffersGeometry(robj->Window, 0, 0, format);

			robj->Surface = eglCreateWindowSurface(robj->Display, robj->Config, robj->Window, eglSurfaceAttrs);
			if (robj->Surface == EGL_NO_SURFACE)
			{
				//checkErrorEGL("eglCreateWindowSurface");
				//goto error;
				assert(false);
				return GY_INVALID_RENDERER_ID;
			}

			//if (setCurrentID(allocableID) == Failed)
			if (eglMakeCurrent(robj->Display, robj->Surface, robj->Surface, robj->Context) != EGL_TRUE)
			{
				//checkErrorEGL("eglMakeCurrent");
				//goto error;
				assert(false);
				return GY_INVALID_RENDERER_ID;
			}

			EGLint queriedWidth = 0, queriedHeight = 0;
			eglQuerySurface(robj->Display, robj->Surface, EGL_WIDTH, &queriedWidth);
			eglQuerySurface(robj->Display, robj->Surface, EGL_HEIGHT, &queriedHeight);

			//@todo: keeping orientation angle
			//if (__width < __height)
				//__orientationAngle = 0;

			// Set vsync.
			//@todo: make configurable of choosing option VSync (0: False, 1: True)
			eglSwapInterval(robj->Display, 1);

			// Initialize OpenGL ES extensions.
			robj->Extensions = (const char *)glGetString(GL_EXTENSIONS);
			if (strstr(robj->Extensions, "GL_OES_vertex_array_object") 
				|| strstr(robj->Extensions, "GL_ARB_vertex_array_object"))
			{
				// Disable VAO extension for now.
				glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
				glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArrays");
				glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
				glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
			}

			setCurrentID(allocableID);
			return allocableID;
		}

		renderer_t createRenderer(const _renderer_object_t &rawRendererContext)
		{
			return Success;
		}

		int destroyRenderer(renderer_t rid)
		{
			return Success;
		}

		uint32 clear_color = 0xFF0000FF;
		uint32 getClearColor()
		{
			return clear_color;
		}

        int setClearColor(const uint32 &color)
		{
			clear_color = color;

			c4f cf((float)((color << 8) >> 24) / 255.0f
				, (float)((color << 16) >> 24) / 255.0f
				, (float)((color << 24) >> 24) / 255.0f
				, (float)(color >> 24) / 255.0f);
			glClearColor(cf.r, cf.g, cf.b, cf.a);

			return Success;
		}
        
		int clear()
		{
			if (isValid() == false)
				return Failed;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
			if (isValid() == false)
			{
				assert(false);
				return Failed;
			}

			const int &rendererAPIType = getRendererAPIType();
			if (rendererAPIType == GY_RENDERER_OPENGLES)
			{
				renderer_object_t robj = &renderer_objects[getCurrentID()];
				eglSwapBuffers(robj->Display, robj->Surface);
				glFlush();
				return Success;
			}

			return Failed;
		}
	}
}
#endif
