#include "gyRenderer.h"
#if defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
// For MS-Windows
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#define GLEW_STATIC
extern "C" 
{
#	include "glew/glew.h"
#	include "glew/glew.c"
}

template <typename _Type_>
int getAllocableBufferID(std::vector<_Type_ *> &c)
{
	for (size_t itr = 0; itr < c.size(); ++itr)
	{
		if (c[itr] == NullPtr)
		{
			c[itr] = new _Type_;
			return itr;
		}
	}

	c.push_back(new _Type_);
	return c.size() - 1;
}

template <typename _Type_>
_Type_ *getValidBuffer(std::vector<_Type_ *> &c, int eid)
{
	if (eid == Invalid)
	{
		assert(false);
		return NullPtr;
	}

	if (c.size() <= (size_t)(eid))
	{
		assert(false);
		return NullPtr;
	}

	if (c[eid] == NullPtr)
	{
		assert(false);
		return NullPtr;
	}

	return c[eid];
}

namespace gy
{
	namespace r3
	{
#define MAX_RENDERER_CONTEXT 8
		_renderer_object_t renderer_objects[MAX_RENDERER_CONTEXT] = {{NullPtr, NullPtr, NullPtr},};
		size_t current_renderer_id = GY_INVALID_RENDERER_ID;
		struct GLAttribute
		{
			GLint Location;
			GLint Type;
		};
		std::unordered_map<std::string, GLAttribute> attribute_map;
		std::unordered_map<std::string, GLint> uniform_map;

		uint32 checkLastError(const char *prefix = "", bool suppressLog = false)
		{
			std::cerr << prefix << " : ";
			uint32 errorCode = glGetError();

			//No error has been recorded. The value of this symbolic constant is guaranteed to be 0.
			if (errorCode == GL_NO_ERROR)
			{
				if (suppressLog == false)
					std::cerr << "No error has been recorded. The value of this symbolic constant is guaranteed to be 0." << std::endl;
			}
			//An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.
			else if (errorCode == GL_INVALID_ENUM)
			{
				if (suppressLog == false)
					std::cerr << "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;

				assert(false);
			}
			//A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.
			else if (errorCode == GL_INVALID_VALUE)
			{
				if (suppressLog == false)
					std::cerr << "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
				
				assert(false);
			}
			//The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.
			else if (errorCode == GL_INVALID_OPERATION)
			{
				if (suppressLog == false)
					std::cerr << "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
				
				assert(false);
			}
			//The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.
			else if (errorCode == GL_INVALID_FRAMEBUFFER_OPERATION)
			{
				if (suppressLog == false)
					std::cerr << "The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
				
				assert(false);
			}
			//There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.
			else if (errorCode == GL_OUT_OF_MEMORY)
			{
				if (suppressLog == false)
					std::cerr << "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded." << std::endl;
				
				assert(false);
			}
			//An attempt has been made to perform an operation that would cause an internal stack to underflow.
			else if (errorCode == GL_STACK_UNDERFLOW)
			{
				if (suppressLog == false)
					std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to underflow." << std::endl;
				
				assert(false);
			}
			//An attempt has been made to perform an operation that would cause an internal stack to overflow.
			else if (errorCode == GL_STACK_OVERFLOW)
			{
				if (suppressLog == false)
					std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to overflow." << std::endl;
				
				assert(false);
			}

			return errorCode;
		}
#define GY_GL_ASSERT(_f_) {_f_; std::string logErr(__FUNCTION__); char strLineNumber[32] = {0}; itoa(__LINE__, strLineNumber, 10); logErr += "(Line="; logErr += strLineNumber; logErr += ")"; checkLastError(logErr.c_str());}

		const char *getStringOfVertexSignature(vertex_signature_type_t vst)
		{
			if (vst == GY_VST_BYTE) return "GY_VST_BYTE";
			else if (vst == GY_VST_UNSIGNED_BYTE) return "GY_VST_UNSIGNED_BYTE";
			else if (vst == GY_VST_SHORT) return "GY_VST_SHORT";
			else if (vst == GY_VST_UNSIGNED_SHORT) return "GY_VST_UNSIGNED_SHORT";
			else if (vst == GY_VST_INT) return "GY_VST_INT";
			else if (vst == GY_VST_UNSIGNED_INT) return "GY_VST_UNSIGNED_INT";
			else if (vst == GY_VST_FLOAT) return "GY_VST_FLOAT";
			else if (vst == GY_VST_2_BYTES) return "GY_VST_2_BYTES";
			else if (vst == GY_VST_3_BYTES) return "GY_VST_3_BYTES";
			else if (vst == GY_VST_4_BYTES) return "GY_VST_4_BYTES";
			else if (vst == GY_VST_DOUBLE) return "GY_VST_DOUBLE";
			else if (vst == GY_VST_FLOAT_VEC2) return "GY_VST_FLOAT_VEC2";
			else if (vst == GY_VST_FLOAT_VEC3) return "GY_VST_FLOAT_VEC3";
			else if (vst == GY_VST_FLOAT_VEC4) return "GY_VST_FLOAT_VEC4";
			else if (vst == GY_VST_INT_VEC2) return "GY_VST_INT_VEC2";
			else if (vst == GY_VST_INT_VEC3) return "GY_VST_INT_VEC3";
			else if (vst == GY_VST_INT_VEC4) return "GY_VST_INT_VEC4";
			else if (vst == GY_VST_BOOL) return "GY_VST_BOOL";
			else if (vst == GY_VST_BOOL_VEC2) return "GY_VST_BOOL_VEC2";
			else if (vst == GY_VST_BOOL_VEC3) return "GY_VST_BOOL_VEC3";
			else if (vst == GY_VST_BOOL_VEC4) return "GY_VST_BOOL_VEC4";
			else if (vst == GY_VST_FLOAT_MAT2) return "GY_VST_FLOAT_MAT2";
			else if (vst == GY_VST_FLOAT_MAT3) return "GY_VST_FLOAT_MAT3";
			else if (vst == GY_VST_FLOAT_MAT4) return "GY_VST_FLOAT_MAT4";
			else if (vst == GY_VST_SAMPLER_1D) return "GY_VST_SAMPLER_1D";
			else if (vst == GY_VST_SAMPLER_2D) return "GY_VST_SAMPLER_2D";
			else if (vst == GY_VST_SAMPLER_3D) return "GY_VST_SAMPLER_3D";
			else if (vst == GY_VST_SAMPLER_CUBE) return "GY_VST_SAMPLER_CUBE";
			else if (vst == GY_VST_SAMPLER_1D_SHADOW) return "GY_VST_SAMPLER_1D_SHADOW";
			else if (vst == GY_VST_SAMPLER_2D_SHADOW) return "GY_VST_SAMPLER_2D_SHADOW";

			return NullPtr;
		}

		size_t getSizeOfVertexSignature(vertex_signature_type_t vst)
		{
			// unit is bytes
			if (vst == GY_VST_BYTE) return 1;
			else if (vst == GY_VST_UNSIGNED_BYTE) return 1;
			else if (vst == GY_VST_SHORT) return 2;
			else if (vst == GY_VST_UNSIGNED_SHORT) return 2;
			else if (vst == GY_VST_INT) return 4;
			else if (vst == GY_VST_UNSIGNED_INT) return 4;
			else if (vst == GY_VST_FLOAT) return 4;
			else if (vst == GY_VST_2_BYTES) return 2;
			else if (vst == GY_VST_3_BYTES) return 3;
			else if (vst == GY_VST_4_BYTES) return 4;
			else if (vst == GY_VST_DOUBLE) return 4;
			else if (vst == GY_VST_FLOAT_VEC2) return 8;
			else if (vst == GY_VST_FLOAT_VEC3) return 12;
			else if (vst == GY_VST_FLOAT_VEC4) return 16;
			else if (vst == GY_VST_INT_VEC2) return 8;
			else if (vst == GY_VST_INT_VEC3) return 12;
			else if (vst == GY_VST_INT_VEC4) return 16;
			else if (vst == GY_VST_BOOL) return 1;
			else if (vst == GY_VST_BOOL_VEC2) return 2;
			else if (vst == GY_VST_BOOL_VEC3) return 3;
			else if (vst == GY_VST_BOOL_VEC4) return 4;
			else if (vst == GY_VST_FLOAT_MAT2) return 16;
			else if (vst == GY_VST_FLOAT_MAT3) return 36;
			else if (vst == GY_VST_FLOAT_MAT4) return 64;
			//@todo: 정확한 수치는? 현재는 다른 예제를 참조하여, Integer 형과 같은 크기로 반환함
			else if (vst == GY_VST_SAMPLER_1D) return 4;
			else if (vst == GY_VST_SAMPLER_2D) return 4;
			else if (vst == GY_VST_SAMPLER_3D) return 4;
			else if (vst == GY_VST_SAMPLER_CUBE) return 4;
			else if (vst == GY_VST_SAMPLER_1D_SHADOW) return 4;
			else if (vst == GY_VST_SAMPLER_2D_SHADOW) return 4;

			return 0;
		}

		vertex_signature_type_t convertOGLToGY(GLint glt)
		{
			if (glt == GL_BYTE) return GY_VST_BYTE;
			else if (glt == GL_UNSIGNED_BYTE) return GY_VST_UNSIGNED_BYTE;
			else if (glt == GL_SHORT) return GY_VST_SHORT;
			else if (glt == GL_UNSIGNED_SHORT) return GY_VST_UNSIGNED_SHORT;
			else if (glt == GL_INT) return GY_VST_INT;
			else if (glt == GL_UNSIGNED_INT) return GY_VST_UNSIGNED_INT;
			else if (glt == GL_FLOAT) return GY_VST_FLOAT;
			else if (glt == GL_2_BYTES) return GY_VST_2_BYTES;
			else if (glt == GL_3_BYTES) return GY_VST_3_BYTES;
			else if (glt == GL_4_BYTES) return GY_VST_4_BYTES;
			else if (glt == GL_DOUBLE) return GY_VST_DOUBLE;
			else if (glt == GL_FLOAT_VEC2) return GY_VST_FLOAT_VEC2;
			else if (glt == GL_FLOAT_VEC3) return GY_VST_FLOAT_VEC3;
			else if (glt == GL_FLOAT_VEC4) return GY_VST_FLOAT_VEC4;
			else if (glt == GL_INT_VEC2) return GY_VST_INT_VEC2;
			else if (glt == GL_INT_VEC3) return GY_VST_INT_VEC3;
			else if (glt == GL_INT_VEC4) return GY_VST_INT_VEC4;
			else if (glt == GL_BOOL) return GY_VST_BOOL;
			else if (glt == GL_BOOL_VEC2) return GY_VST_BOOL_VEC2;
			else if (glt == GL_BOOL_VEC3) return GY_VST_BOOL_VEC3;
			else if (glt == GL_BOOL_VEC4) return GY_VST_BOOL_VEC4;
			else if (glt == GL_FLOAT_MAT2) return GY_VST_FLOAT_MAT2;
			else if (glt == GL_FLOAT_MAT3) return GY_VST_FLOAT_MAT3;
			else if (glt == GL_FLOAT_MAT4) return GY_VST_FLOAT_MAT4;
			else if (glt == GL_SAMPLER_1D) return GY_VST_SAMPLER_1D;
			else if (glt == GL_SAMPLER_2D) return GY_VST_SAMPLER_2D;
			else if (glt == GL_SAMPLER_3D) return GY_VST_SAMPLER_3D;
			else if (glt == GL_SAMPLER_CUBE) return GY_VST_SAMPLER_CUBE;
			else if (glt == GL_SAMPLER_1D_SHADOW) return GY_VST_SAMPLER_1D_SHADOW;
			else if (glt == GL_SAMPLER_2D_SHADOW) return GY_VST_SAMPLER_2D_SHADOW;

			return 0;
		}

		GLint convertGYToOGL(vertex_signature_type_t vst)
		{
			if (vst == GY_VST_BYTE) return GL_BYTE;
			else if (vst == GY_VST_UNSIGNED_BYTE) return GL_UNSIGNED_BYTE;
			else if (vst == GY_VST_SHORT) return GL_SHORT;
			else if (vst == GY_VST_UNSIGNED_SHORT) return GL_UNSIGNED_SHORT;
			else if (vst == GY_VST_INT) return GL_INT;
			else if (vst == GY_VST_UNSIGNED_INT) return GL_UNSIGNED_INT;
			else if (vst == GY_VST_FLOAT) return GL_FLOAT;
			else if (vst == GY_VST_2_BYTES) return GL_2_BYTES;
			else if (vst == GY_VST_3_BYTES) return GL_3_BYTES;
			else if (vst == GY_VST_4_BYTES) return GL_4_BYTES;
			else if (vst == GY_VST_DOUBLE) return GL_DOUBLE;
			else if (vst == GY_VST_FLOAT_VEC2) return GL_FLOAT_VEC2;
			else if (vst == GY_VST_FLOAT_VEC3) return GL_FLOAT_VEC3;
			else if (vst == GY_VST_FLOAT_VEC4) return GL_FLOAT_VEC4;
			else if (vst == GY_VST_INT_VEC2) return GL_INT_VEC2;
			else if (vst == GY_VST_INT_VEC3) return GL_INT_VEC3;
			else if (vst == GY_VST_INT_VEC4) return GL_INT_VEC4;
			else if (vst == GY_VST_BOOL) return GL_BOOL;
			else if (vst == GY_VST_BOOL_VEC2) return GL_BOOL_VEC2;
			else if (vst == GY_VST_BOOL_VEC3) return GL_BOOL_VEC3;
			else if (vst == GY_VST_BOOL_VEC4) return GL_BOOL_VEC4;
			else if (vst == GY_VST_FLOAT_MAT2) return GL_FLOAT_MAT2;
			else if (vst == GY_VST_FLOAT_MAT3) return GL_FLOAT_MAT3;
			else if (vst == GY_VST_FLOAT_MAT4) return GL_FLOAT_MAT4;
			else if (vst == GY_VST_SAMPLER_1D) return GL_SAMPLER_1D;
			else if (vst == GY_VST_SAMPLER_2D) return GL_SAMPLER_2D;
			else if (vst == GY_VST_SAMPLER_3D) return GL_SAMPLER_3D;
			else if (vst == GY_VST_SAMPLER_CUBE) return GL_SAMPLER_CUBE;
			else if (vst == GY_VST_SAMPLER_1D_SHADOW) return GL_SAMPLER_1D_SHADOW;
			else if (vst == GY_VST_SAMPLER_2D_SHADOW) return GL_SAMPLER_2D_SHADOW;

			return 0;
		}

		renderer_t getAllocableID()
		{
			for (size_t i = 0; i < MAX_RENDERER_CONTEXT; ++i)
			{
				if (isValidRenderer(i) == true)
					continue;

				return i;
			}

			return GY_INVALID_RENDERER_ID;
		}

		int setCurrentRendererID(renderer_t rid)
		{
			if (rid == GY_INVALID_RENDERER_ID)
			{
				return Failed;
			}

			current_renderer_id = rid;
			renderer_object_t robj = &renderer_objects[rid];
			if (isValidRenderer(rid) == false)
			{
				return Failed;
			}

			const int &rendererAPIType = getRendererAPIType();
			if (rendererAPIType == GY_RENDERER_OPENGL)
			{
				if (wglMakeCurrent(robj->DeviceContext, robj->RendererContext))
					return Success;
			}

			return Failed;
		}

		renderer_t getCurrentRendererID()
		{
			return current_renderer_id;
		}

		renderer_object_t getRendererObject(renderer_t rid)
		{
			if (isValidRenderer(rid) == false)
				return NullPtr;

			return &renderer_objects[rid];
		}

		uint32 getMaxAllocableObjectCount()
		{
			return MAX_RENDERER_CONTEXT;
		}

		bool isValidRenderer()
		{
			const renderer_t rid = getCurrentRendererID();
			HGLRC currentRC = wglGetCurrentContext();
			bool ret = isValidRenderer(rid) == true && renderer_objects[rid].RendererContext == currentRC;
			return ret;
		}

		bool isValidRenderer(const int rid)
		{
			if (rid == GY_INVALID_RENDERER_ID)
				return false;

			int rendererAPIType = getRendererAPIType();
			renderer_object_t robj = &renderer_objects[rid];
			bool ret = rendererAPIType != GY_RENDERER_UNKNOWN 
				&& rendererAPIType < GY_RENDERER_TYPE_SIZE 
				&& robj != NullPtr
				&& robj->Window != NULL
				&& robj->DeviceContext != NULL
				&& robj->RendererContext != NULL;
			//&& robj->RendererContext != wglGetCurrentContext();

			return ret;
		}

		// OS별 특징이 있는 공통 함수들
		size_t screen_width = 1;
		size_t screen_height = 1;
		renderer_t createRenderer(void *_vid, size_t width, size_t height)
		{
			const renderer_t allocableID = getAllocableID();
			if (allocableID == GY_INVALID_RENDERER_ID)
			{
				assert(false);
				return GY_INVALID_RENDERER_ID;
			}
			renderer_object_t robj = &renderer_objects[allocableID];

			robj->Window = (HWND)_vid;
			const int rendererAPIType = getRendererAPIType();
			if (rendererAPIType == GY_RENDERER_UNKNOWN)
				return GY_INVALID_RENDERER_ID;

			if (rendererAPIType == GY_RENDERER_OPENGL)
			{
				PIXELFORMATDESCRIPTOR pixelFormatDesc;
				memset(&pixelFormatDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
				pixelFormatDesc.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
				pixelFormatDesc.nVersion   = 1;
				pixelFormatDesc.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
				pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
				pixelFormatDesc.cColorBits = 32;
				pixelFormatDesc.cDepthBits = 32;
				pixelFormatDesc.iLayerType = PFD_MAIN_PLANE;

				robj->DeviceContext = GetDC(robj->Window);
				int pixelFormat = ChoosePixelFormat(robj->DeviceContext, &pixelFormatDesc);
				BOOL ret = SetPixelFormat(robj->DeviceContext, pixelFormat, &pixelFormatDesc);
				robj->RendererContext = wglCreateContext(robj->DeviceContext);
				ret = wglMakeCurrent(robj->DeviceContext, robj->RendererContext);
				if (glewInit() != GLEW_OK)
				{
					assert(false);
					return GY_INVALID_RENDERER_ID;
				}

				GLint majorVersion = -1, minorVersion = -1;
				glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
				glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
				printf("OpenGL Version %d.%d\n", majorVersion, minorVersion);

				if (getCurrentRendererID() == GY_INVALID_RENDERER_ID)
					setCurrentRendererID(allocableID);

				glEnable(GL_DEPTH_TEST);
				glViewport(0, 0, width, height);
				screen_width = width;
				screen_height = height;

				return allocableID;
			}
			else if (rendererAPIType == GY_RENDERER_OPENGLES)
			{
			}
			else if (rendererAPIType == GY_RENDERER_DIRECT3D)
			{
			}

			return GY_INVALID_RENDERER_ID;
		}

		renderer_t createRenderer(const _renderer_object_t &rawRendererContext)
		{
			if (isValidRenderer() == false)
				return GY_INVALID_RENDERER_ID;

			renderer_t rid = GY_INVALID_RENDERER_ID;

			const int &rendererAPIType = getRendererAPIType();
			if (rendererAPIType == GY_RENDERER_OPENGL)
			{
				//rid = &rawRendererContext;
			}
			else if (rendererAPIType == GY_RENDERER_OPENGLES)
			{
			}
			else if (rendererAPIType == GY_RENDERER_DIRECT3D)
			{
			}

			return rid;
		}

		int destroyRenderer(renderer_t rid)
		{
			if (isValidRenderer(rid) == false)
				return Failed;

			const int &rendererAPIType = getRendererAPIType();
			if (rendererAPIType == GY_RENDERER_OPENGL)
			{
			}
			else if (rendererAPIType == GY_RENDERER_OPENGLES)
			{
			}
			else if (rendererAPIType == GY_RENDERER_DIRECT3D)
			{
			}

			return Failed;
		}

		uint getScreenWidth()
		{
			return screen_width;
		}

		uint getScreenHeight()
		{
			return screen_height;
		}

		uint32 clear_color = 0xFF0000FF;
		uint32 getClearColor()
		{
			return clear_color;
		}

		int setClearColor(const uint32 &color)
		{
			clear_color = color;

			gy::math::c4f cf((float)((color << 8) >> 24) / 255.0f
				, (float)((color << 16) >> 24) / 255.0f
				, (float)((color << 24) >> 24) / 255.0f
				, (float)(color >> 24) / 255.0f);
			glClearColor(cf.r, cf.g, cf.b, cf.a);

			return Success;
		}

		int clear()
		{
			if (isValidRenderer() == false)
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
			if (isValidRenderer() == false)
			{
				assert(false);
				return Failed;
			}

			const int &rendererAPIType = getRendererAPIType();
			if (rendererAPIType == GY_RENDERER_OPENGL)
			{
				glFlush();

				HDC context = wglGetCurrentDC();
				SwapBuffers(context);

				return Success;
			}
			else if (rendererAPIType == GY_RENDERER_OPENGLES)
			{
			}
			else if (rendererAPIType == GY_RENDERER_DIRECT3D)
			{
			}

			return Failed;
		}

		// 버텍스 버퍼
		struct vertex_buffer_object_t
		{
			vertex_buffer_t VertexBufferID; ///< 이 객체를 가리키는 ID
			uint VertexArrayID; ///< 실제 정점 버퍼의 객체
			uint VertexBufferObjectID; ///< 실제 정점 버퍼의 객체
			vertex_format_t VertexElementFormatID; ///< vertex_signature_t의 Element를 가리기는 ID
			uint32 VerticesCount;

			vertex_buffer_object_t() 
				: VertexBufferID(GY_INVALID_VERTEX_BUFFER_ID)
				//, VertexArrayID(GL_INVALID_VALUE)
				//, VertexBufferObjectID(GL_INVALID_VALUE)
				, VertexElementFormatID(GY_INVALID_VERTEX_FORMAT_ID) 
				, VerticesCount(0)
			{}
			virtual ~vertex_buffer_object_t()
			{}
		};
		std::vector<vertex_buffer_object_t *> vertex_buffer_objects;
		typedef std::vector<vertex_layout_t> vertex_layout_container_t;
		/// vertex attribute의 타입 조합을 생성할 때 사용할 정보. (glGetAttribLocation에 쓰일 program ID)
		struct vertex_layout_context_t
		{
			vertex_layout_container_t LayoutContexts;
			uint32 Stride;
		};
		std::vector<vertex_layout_context_t *> vertex_layout_contexts;

		size_t getSizeOfVertexFormat(vertex_format_t vfid)
		{
			// vfid의 vertex_element_signature가 유효한지 검사
			vertex_layout_context_t *vasobj = getValidBuffer<vertex_layout_context_t>(vertex_layout_contexts, vfid);
			if (vasobj == NullPtr)
			{
				assert(false && "invalid vertex format ID.");
				return 1;
			}

			return vasobj->Stride;
		}

		int adjustVertexAttributePointer(vertex_format_t vertexFormatID)
		{
			// vfid의 vertex_element_signature가 유효한지 검사
			vertex_layout_context_t *vasobj = getValidBuffer<vertex_layout_context_t>(vertex_layout_contexts, vertexFormatID);
			if (vasobj == NullPtr)
			{
				assert(false && "Invalid Vertex Element ID");
				return Failed;
			}

			checkLastError();

			size_t offset = 0;
			uint32 stride = vasobj->Stride;
			vertex_layout_container_t &vesArray = vasobj->LayoutContexts;
			for (size_t i = 0; i < vesArray.size(); ++i)
			{
				vertex_layout_t vef = vesArray[i];

				auto itr = attribute_map.find(vef.Signature);
				if (itr == attribute_map.end())
					continue;

				GLAttribute attribute = itr->second;
				GLint location = attribute.Location;
				GLint type = attribute.Type;
				GLuint size = vef.Size; // size of elements (count)
				size_t vstSize = getSizeOfVertexSignature(vef.Type); // size of type
				size_t stride = getSizeOfVertexFormat(vertexFormatID);
				//@todo: vef.Type을 실제 GL의 Type으로 변환하는 기능 필요.
				std::cerr << "Name = " << vef.Signature << ", Location = " << location 
					<< ", Type = " << getStringOfVertexSignature(convertOGLToGY(type)) << "(" << type << ")"
					<< ", Size = " << vstSize << ", Stride = " << stride << std::endl;
				//why this context has been error when type wasn't GL_FLOAT?
				GY_GL_ASSERT( glVertexAttribPointer(location, vstSize < 4 ? vstSize : vstSize / 4, GL_FLOAT, GL_FALSE, stride, (char *)(0 + offset)) );
				GY_GL_ASSERT( glEnableVertexAttribArray(location) );
				offset += vstSize;
			}

			return Success;
		}

		vertex_buffer_t createVertexBuffer(const void *vertexBuffer, uint32 dataSize, const vertex_format_t vertexFormatID, int usage)
		{
			int allocableID = getAllocableBufferID<vertex_buffer_object_t>(vertex_buffer_objects);
			if (allocableID == Invalid)
				return GY_INVALID_VERTEX_BUFFER_ID;

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			vertex_buffer_object_t *vbo = vertex_buffer_objects[allocableID];
			vbo->VertexBufferID = allocableID;
			vbo->VertexElementFormatID = vertexFormatID;

			GLuint vertexArrayID = GL_INVALID_VALUE;
			glGenVertexArrays(1, &vertexArrayID);
			if (vertexArrayID == GL_INVALID_VALUE)
				return Failed;

			glBindVertexArray(vertexArrayID);

			GLuint vertexBufferObjectID = GL_INVALID_VALUE;
			glGenBuffers(1, &vertexBufferObjectID);
			if (vertexBufferObjectID == GL_INVALID_VALUE)
				return Failed;

			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);

			size_t sizeOfVertexElementFormat = getSizeOfVertexFormat(vertexFormatID);
			glBufferData(GL_ARRAY_BUFFER, dataSize, vertexBuffer, GL_STATIC_DRAW/*usage*/);

			if (adjustVertexAttributePointer(vertexFormatID) == Failed)
				return Failed;

			vbo->VertexArrayID = vertexArrayID;
			vbo->VertexBufferObjectID = vertexBufferObjectID;
			vbo->VerticesCount = dataSize / sizeOfVertexElementFormat;
			glBindVertexArray(0);

			return allocableID;
		}

		vertex_format_t createVertexFormat(const vertex_layout_t *vertexLayoutArray, uint32 dataSize)
		{
			//if (structureName == NullPtr)
			//{
			//	assert(false && "Invalid structure name of vertex element signatures.");
			//	return GY_INVALID_VERTEX_FORMAT_ID;
			//}

			if (vertexLayoutArray == NullPtr)
			{
				assert(false && "Invalid vertex element signatures.");
				return GY_INVALID_VERTEX_FORMAT_ID;
			}

			vertex_format_t allocableID = getAllocableBufferID<vertex_layout_context_t>(vertex_layout_contexts);
			if (allocableID == Invalid)
				return GY_INVALID_VERTEX_FORMAT_ID;

			vertex_layout_context_t *vasobj = vertex_layout_contexts[allocableID];
			size_t stride = 0;
			uint32 elementCount = dataSize / sizeof(vertex_layout_t);
			for (size_t i = 0; i < elementCount; ++i)
			{
				vertex_layout_t vef = vertexLayoutArray[i];
				int vs = vef.SignID;
				int type = vef.Type;
				if (vs == GY_VS_UNKNOWN)
					break;

				size_t len = getSizeOfVertexSignature(type);
				if (len == 0)
				{
					delete vasobj;
					vasobj = NullPtr;
					return GY_INVALID_VERTEX_FORMAT_ID;
				}

				stride += len;

				vasobj->LayoutContexts.push_back(vertexLayoutArray[i]);
			}
			vasobj->Stride = stride;
			
			return allocableID;
		}

		// 인덱스 버퍼
		struct index_buffer_object_t
		{
			GLuint ID;
			uint32 IndicesCount;
			uint32 SizeOfIndexElementType;
		};
		std::vector<index_buffer_object_t *> index_buffer_objects;

		index_buffer_t createIndexBuffer(uint32 sizeOfElementType, uint32 elementCount, const void *indexBuffer, int usage)
		{
			if (indexBuffer == NullPtr || sizeOfElementType < 2 || elementCount <= 0)
			{
				assert(false && "Invalid index buffer or size of element type or element count.");
				return GY_INVALID_INDEX_BUFFER_ID;
			}

			index_buffer_t allocableID = getAllocableBufferID<index_buffer_object_t>(index_buffer_objects);
			if (allocableID == Invalid)
				return GY_INVALID_INDEX_BUFFER_ID;

			index_buffer_object_t *iobj = index_buffer_objects[allocableID];
			glGenBuffers(1, &(iobj->ID));
			if (iobj->ID == 0)
			{
				assert(false);
				delete iobj;
				iobj = NullPtr;
				return GY_INVALID_INDEX_BUFFER_ID;
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iobj->ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeOfElementType, indexBuffer, GL_STATIC_DRAW/*usage*/);
			iobj->IndicesCount = elementCount;
			iobj->SizeOfIndexElementType = sizeOfElementType;

			return allocableID;
		}

		// 텍스쳐 버퍼
		struct texture_object_t
		{
			GLuint ID;
			char *FileName;
			uint32 Width;
			uint32 Height;
		};
		std::vector<texture_object_t *> texture_objects;

		texture_t createTextureFromFile(const char *fileName, uint32 width, uint32 height)
		{
			int w, h, c;
			unsigned char *ht_map = SOIL_load_image(fileName, &w, &h, &c, SOIL_LOAD_L);

			if (width == _Default)
				width = w;

			if (height == _Default)
				height = h;

			texture_t allocableTextureID = getAllocableBufferID<texture_object_t>(texture_objects);
			texture_object_t *tobj = texture_objects[allocableTextureID];
			const size_t filenameLen = strlen(fileName);
			tobj->FileName = new char[filenameLen];
			strcpy_s(tobj->FileName, filenameLen, fileName);
			tobj->Width = width == _Default ? w : width;
			tobj->Height = height == _Default ? h : height;
			tobj->ID = SOIL_create_OGL_texture(
				ht_map,
				width, 
				height, 
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
				);

			if (tobj->ID == 0)
			{
				delete [](tobj->FileName);
				delete tobj;
				tobj = NullPtr;
				return GY_INVALID_TEXTURE_ID;
			}

			return allocableTextureID;
		}

		texture_t createTextureFromMemory(void *rawBuffer, uint32 width, uint height)
		{
			texture_t allocableTextureID = getAllocableBufferID<texture_object_t>(texture_objects);
			texture_object_t *tobj = texture_objects[allocableTextureID];
			tobj->FileName = NullPtr;
			tobj->Width = width;
			tobj->Height = height;
			tobj->ID = SOIL_create_OGL_texture(
				(unsigned char *)rawBuffer, 
				width, 
				height, 
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
				);

			if (tobj->ID == 0)
			{
				delete tobj;
				tobj = NullPtr;
				return GY_INVALID_TEXTURE_ID;
			}

			return allocableTextureID;
		}

		int bindTexture(texture_t tid)
		{
			texture_object_t *tobj = getValidBuffer<texture_object_t>(texture_objects, tid); 
			if (tobj == NullPtr)
			{
				assert(false && "Invalid Texture ID");
				return Failed;
			}

			glBindTexture(GL_TEXTURE_2D, tobj->ID);

			return Success;
		}

		int destroyTexture(texture_t tid)
		{
			texture_object_t *tobj = getValidBuffer<texture_object_t>(texture_objects, tid); 
			if (tobj == NullPtr)
			{
				assert(false && "Invalid Texture ID");
				return Failed;
			}

			glDeleteTextures(1, &(tobj->ID));

			if (tobj->FileName != NullPtr)
			{
				delete [](tobj->FileName);
			}

			delete tobj;
			tobj = NullPtr;

			return Success;
		}

		// 셰이더
		/// 셰이더 프로그램 객체
		struct program_object_t
		{
			GLuint ID;
		};
		std::vector<program_object_t *> program_objects;
		static GLuint current_gl_program_id = 0; // Note! Invalid Value is 0 not Invalid

		/// 셰이더 객체
		struct shader_object_t
		{
			GLuint ShaderID;
			shader_type_e ShaderType;
			program_t ProgramID;
		};
		std::vector<shader_object_t *> shader_objects; 

		int linkShaderProgram(program_t pid)
		{
			// 해당 shader program을 알아낸다.
			program_object_t *pobj = getValidBuffer<program_object_t>(program_objects, pid);
			if (pobj == NullPtr)
			{
				assert(false);
				return Failed;
			}
			GLuint program = pobj->ID;

			// glLinkProgram을 수행한다.
			glLinkProgram(program);

			GLint length = 0;
#if defined(DEBUG) || defined(_DEBUG)
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			if (length > 0) 
			{
				GLchar *log = (GLchar *)malloc(length);
				glGetProgramInfoLog(program, length, &length, log);
				//NSLog(@"Program link log:\n%s", log);
				printf("Program link log:\n%s\n", log);
				free(log);
			}
#endif

			GLint status;
			glGetProgramiv(program, GL_LINK_STATUS, &status);
			if (status == 0) 
			{
				return Failed;
			}

			// Query and store vertex attribute meta-data from the program.
			// NOTE: Rather than using glBindAttribLocation to explicitly specify our own
			// preferred attribute locations, we're going to query the locations that were
			// automatically bound by the GPU. While it can sometimes be convenient to use
			// glBindAttribLocation, some vendors actually reserve certain attribute indices
			// and therefore using this function can create compatibility issues between
			// different hardware vendors.
			attribute_map.clear();
			length = 0;
			GLint activeAttributes;
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
			if (activeAttributes > 0)
			{
				glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length);
				if (length > 0)
				{
					GLchar* attribName = new GLchar[length + 1];
					GLint attribSize;
					GLenum attribType;
					GLint attribLocation;
					for (int i = 0; i < activeAttributes; ++i)
					{
						// Query attribute info.
						glGetActiveAttrib(program, i, length, NULL, &attribSize, &attribType, attribName);
						attribName[length] = '\0';

						// Query the pre-assigned attribute location.
						attribLocation = glGetAttribLocation(program, attribName);

						// Assign the vertex attribute mapping for the effect.
						GLAttribute attribute;
						attribute.Location = attribLocation;
						attribute.Type = attribType;
						attribute_map[attribName] = attribute;

						std::cout << "* Attribute Name = " << attribName << ", Location = " << attribute.Location << ", Size = " << attribSize << ", Type = " << attribute.Type << std::endl;
					}
					delete []attribName;
				}
			}

			// Query and store uniforms from the program.
			uniform_map.clear();
			length = 0;
			GLint activeUniforms;
			glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms);
			if (activeUniforms > 0)
			{
				glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
				if (length > 0)
				{
					GLchar* uniformName = new GLchar[length + 1];
					GLint uniformSize;
					GLenum uniformType;
					GLint uniformLocation;
					unsigned int samplerIndex = 0;
					for (int i = 0; i < activeUniforms; ++i)
					{
						// Query uniform info.
						glGetActiveUniform(program, i, length, NULL, &uniformSize, &uniformType, uniformName);
						uniformName[length] = '\0';  // null terminate
						if (uniformSize > 1 && length > 3)
						{
							// This is an array uniform. I'm stripping array indexers off it since GL does not
							// seem to be consistent across different drivers/implementations in how it returns
							// array uniforms. On some systems it will return "u_matrixArray", while on others
							// it will return "u_matrixArray[0]".
							char* c = strrchr(uniformName, '[');
							if (c)
							{
								*c = '\0';
							}
						}

						// Query the pre-assigned uniform location.
						uniformLocation = glGetUniformLocation(program, uniformName);
						uniform_map[uniformName] = uniformLocation;
						/*
						Uniform* uniform = new Uniform();
						uniform->_effect = effect;
						uniform->_name = uniformName;
						uniform->_location = uniformLocation;
						uniform->_type = uniformType;
						uniform->_index = uniformType == GL_SAMPLER_2D ? (samplerIndex++) : 0;

						effect->_uniforms[uniformName] = uniform;
						*/
						std::cout << "* Uniform Name = " << uniformName << ", Location = " << uniformLocation << ", Size = " << uniformSize << ", Type = " << uniformType << std::endl;
					}
					delete []uniformName;
				}
			}

			return Success;
		}

		program_t createShaderProgram()
		{
			program_t allocableProgramID = getAllocableBufferID<program_object_t>(program_objects);
			program_object_t *pobj = program_objects[allocableProgramID];
			current_gl_program_id = pobj->ID = glCreateProgram();

			return allocableProgramID;
		}

		shader_t createShaderFromMemory(program_t pid, shader_type_e shaderProgramType, const char *buffer, program_object_t *pobj)
		{
			if (pobj == NullPtr)
			{
				program_object_t *pobj = getValidBuffer<program_object_t>(program_objects, pid);
			}

			if (pobj == NullPtr)
			{
				assert(false && "Invalid Program ID");
				return GY_INVALID_SHADER_ID;
			}

			if (pobj->ID == 0)
			{
				assert(false && "Invalid Program ID");
				return GY_INVALID_SHADER_ID;
			}

			shader_t allocableShaderID = getAllocableBufferID<shader_object_t>(shader_objects);
			shader_object_t *sobj = shader_objects[allocableShaderID];
			sobj->ProgramID = pid;
			switch (shaderProgramType)
			{
			case GY_SHADER_VERTEX:
				{
					sobj->ShaderType = GY_SHADER_VERTEX;
#ifndef GL_VERTEX_SHADER
					const uint32 GL_VERTEX_SHADER = 0x8B31;
#endif
					sobj->ShaderID = glCreateShader(GL_VERTEX_SHADER);
				} break;
			case GY_SHADER_GEOMETRY:
				{
					sobj->ShaderType = GY_SHADER_GEOMETRY;
#ifndef GL_GEOMETRY_SHADER
					const uint32 GL_GEOMETRY_SHADER = 0x8DD9;
#endif
					sobj->ShaderID = glCreateShader(GL_GEOMETRY_SHADER);
				} break;
			case GY_SHADER_FRAGMENT:
				{
					sobj->ShaderType = GY_SHADER_FRAGMENT;
#ifndef GL_FRAGMENT_SHADER
					const uint32 GL_FRAGMENT_SHADER = 0x8B30;
#endif
					sobj->ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
				} break;
			}

			if (sobj->ShaderID == 0 || sobj->ShaderID == GL_INVALID_ENUM)
			{
				delete sobj;
				sobj = NullPtr;

				return GY_INVALID_SHADER_ID;
			}

			GLint bufferLength = strlen(buffer);
			glShaderSource(sobj->ShaderID, 1, &buffer, &bufferLength);
			glCompileShader(sobj->ShaderID);

#if defined(DEBUG) || defined(_DEBUG)
			GLint logLength;
			glGetShaderiv(sobj->ShaderID, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0) 
			{
				GLchar *log = (GLchar *)malloc(logLength);
				glGetShaderInfoLog(sobj->ShaderID, logLength, &logLength, log);
				//NSLog(@"Shader compile log:\n%s", log);
				printf("Shader compile log:\n%s\n", log);
				free(log);
			}
#endif

			GLint status;
			glGetShaderiv(sobj->ShaderID, GL_COMPILE_STATUS, &status);
			if (status == 0) 
			{
				glDeleteShader(sobj->ShaderID);
				delete sobj;
				sobj = NullPtr;

				return GY_INVALID_SHADER_ID;
			}

			glAttachShader(pobj->ID, sobj->ShaderID);
			if (linkShaderProgram(pid) == Failed)
			{
				assert(false && "Failed to link program with a shader context.");
				return GY_INVALID_SHADER_ID;
			}

			return allocableShaderID;
		}

		shader_t createShaderFromFile(program_t pid, shader_type_e shaderProgramType, const char *fileName)
		{
			program_object_t *pobj = getValidBuffer<program_object_t>(program_objects, pid);
			if (pobj == NullPtr)
			{
				assert(false && "Invalid Program ID");
				return GY_INVALID_SHADER_ID;
			}

			if (pobj->ID == 0)
			{
				assert(false && "Invalid Program ID");
				return GY_INVALID_SHADER_ID;
			}

			/*
			std::ifstream shaderFile(fileName);
			std::string buffer;
			while (shaderFile.is_open())
			{
#define MAX_READ_BLOCK 64
				char buf[MAX_READ_BLOCK] = {0};
				shaderFile.read(buf, MAX_READ_BLOCK);
				if (shaderFile.gcount() == 0)
					break;

				buffer += buf;
			}
			*/

			std::string buffer;
			//fixed by that solution has in //http://stackoverflow.com/questions/12117720/reading-file-corrupted-data
			std::ifstream ifs(fileName, std::ios::binary);
			if (!std::getline(ifs, buffer, '\0'))
				std::cerr << "Warning: reading error possible!\n";
			// below context has read wrong data from file. // why?
			/*
			size_t nRead;
			FILE *src = NullPtr;
			fopen_s(&src, fileName,"r");
			if (src != NULL) 
			{
				while (!feof(src)) 
				{
					char block[256] = {0};
					nRead = fread(block, 1, 256, src);
					buffer += block;
				}

				fclose(src);
			}
			*/
			return createShaderFromMemory(pid, shaderProgramType, buffer.c_str(), pobj);
		}

		shader_t createShaderFromMemory(program_t pid, shader_type_e shaderProgramType, const char *buffer)
		{
			return createShaderFromMemory(pid, shaderProgramType, buffer, NullPtr);
		}

		int beginShader(program_t pid)
		{
			program_object_t *pobj = getValidBuffer<program_object_t>(program_objects, pid);
			if (pobj == NullPtr)
			{
				assert(false && "Invalid Program ID");
				return Failed;
			}

			current_gl_program_id = pobj->ID;
			if (current_gl_program_id == 0)
			{
				assert(false && "Invalid Program ID");
				return Failed;
			}

			GLint status;
			glGetProgramiv(current_gl_program_id, GL_LINK_STATUS, &status);
			if (status == 0) 
			{
				return Failed;
			}

			glUseProgram(current_gl_program_id);

			return Success;
		}

		int endShader()
		{
			glUseProgram(0);
			current_gl_program_id = 0;

			return Success;
		}

		//@todo: uniform 갯수 제한에 대한 정책은 아직 정해지지 않음
		int setShaderVariableFloat(const char *variableName, float v0)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniform1f(location, v0);

			return Success;
		}

		int setShaderVariableFloat(const char *variableName, float v0, float v1)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniform2f(location, v0, v1);

			return Success;
		}

		int setShaderVariableFloat(const char *variableName, float v0, float v1, float v2)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniform3f(location, v0, v1, v2);

			return Success;
		}

		int setShaderVariableFloat(const char *variableName, float v0, float v1, float v2, float v3)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniform4f(location, v0, v1, v2, v3);

			return Success;
		}

		int setShaderVariableVector(const char *variableName, const gy::math::vec2 &v)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniform2fv(location, 1, &(v[0]));

			return Success;
		}

		int setShaderVariableVector(const char *variableName, const gy::math::vec3 &v)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniform3fv(location, 1, &(v[0]));

			return Success;
		}

		int setShaderVariableVector(const char *variableName, const gy::math::vec4 &v)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniform4fv(location, 1, &(v[0]));

			return Success;
		}

		int setShaderVariableMatrix(const char *variableName, const gy::math::mat2x2 &v)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniformMatrix2fv(location, 1, GL_FALSE, &(v[0][0]));

			return Success;
		}

		int setShaderVariableMatrix(const char *variableName, const gy::math::mat3x3 &v)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniformMatrix3fv(location, 1, GL_FALSE, &v[0][0]);

			return Success;
		}

		int setShaderVariableMatrix(const char *variableName, const gy::math::mat4x4 &v)
		{
			auto itr = uniform_map.find(variableName);
			if (itr == uniform_map.end())
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			GLint location = itr->second;
			glUniformMatrix4fv(location, 1, GL_FALSE, &v[0][0]);

			return Success;
		}


		int setShaderVariableFloat(program_t pid, const char *variableName, float v0)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName); 
			if (location == Invalid)
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			glProgramUniform1f(pid, location, v0);

			return Success;
		}

		int setShaderVariableFloat(program_t pid, const char *variableName, float v0, float v1)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName); 
			if (location == Invalid)
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			glProgramUniform2f(pid, location, v0, v1);

			return Success;
		}

		int setShaderVariableFloat(program_t pid, const char *variableName, float v0, float v1, float v2)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName); 
			if (location == Invalid)
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			glProgramUniform3f(pid, location, v0, v1, v2);

			return Success;
		}

		int setShaderVariableFloat(program_t pid, const char *variableName, float v0, float v1, float v2, float v3)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName); 
			if (location == Invalid)
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			glProgramUniform4f(pid, location, v0, v1, v2, v3);

			return Success;
		}

		int setShaderVariableVector(program_t pid, const char *variableName, const gy::math::vec2 &v)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName); 
			if (location == Invalid)
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			glProgramUniform2fv(pid, location, 1, &(v[0]));

			return Success;
		}

		int setShaderVariableVector(program_t pid, const char *variableName, const gy::math::vec3 &v)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName); 
			if (location == Invalid)
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			glProgramUniform3fv(pid, location, 1, &(v[0]));

			return Success;
		}

		int setShaderVariableVector(program_t pid, const char *variableName, const gy::math::vec4 &v)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName);
			if (location == Invalid)
				return Failed;

			glProgramUniform4fv(pid, location, 1, &(v[0]));

			return Success;
		}

		int setShaderVariableMatrix(program_t pid, const char *variableName, const gy::math::mat2x2 &v)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName); 
			if (location == Invalid)
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			glProgramUniformMatrix2fv(pid, location, 1, GL_FALSE, &(v[0][0]));

			return Success;
		}

		int setShaderVariableMatrix(program_t pid, const char *variableName, const gy::math::mat3x3 &v)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName); 
			if (location == Invalid)
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			glProgramUniformMatrix3fv(pid, location, 1, GL_FALSE, &v[0][0]);

			return Success;
		}

		int setShaderVariableMatrix(program_t pid, const char *variableName, const gy::math::mat4x4 &v)
		{
			if (pid == 0)
			{
				std::cerr << "Invalid program ID. " << pid << std::endl;
				assert(false && "Invalid program ID.");
				return Failed;
			}

			GLint location = glGetUniformLocation(pid, variableName); 
			if (location == Invalid)
			{
				std::cerr << "Invalid uniform varialble name is " << variableName << std::endl;
				assert(false && "Invalid uniform varialble name.");
				return Failed;
			}

			glProgramUniformMatrix4fv(pid, location, 1, GL_FALSE, &v[0][0]);

			return Success;
		}

		// 그리기
		int draw(draw_type_e drawType, vertex_buffer_t vbid)
		{
			vertex_buffer_object_t *vobj = getValidBuffer<vertex_buffer_object_t>(vertex_buffer_objects, vbid);
			if (vobj == NullPtr)
			{
				assert(false && "Invalid vertex buffer ID");
				return Failed;
			}

			if (vobj->VertexBufferID == Invalid)
			{
				assert(false && "Invalid vertex buffer ID");
				return Failed;
			}

			if (vobj->VertexArrayID == GL_INVALID_VALUE)
			{
				assert(false && "Invalid vertex array ID");
				return Failed;
			}

			glBindVertexArray(vobj->VertexArrayID);

			GLuint dt = drawType == GY_DRAW_TRIANGLE ? GL_TRIANGLES 
				: drawType == GY_DRAW_TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLE_FAN;

			glDrawArrays(dt, 0, vobj->VerticesCount);

			return Success;
		}

		int drawIndexed(draw_type_e drawType, vertex_buffer_t vbid, index_buffer_t ibid)
		{
			vertex_buffer_object_t *vobj = getValidBuffer<vertex_buffer_object_t>(vertex_buffer_objects, vbid);
			if (vobj == NullPtr)
			{
				assert(false && "Invalid vertex buffer ID");
				return Failed;
			}

			if (vobj->VertexBufferID == Invalid)
			{
				assert(false && "Invalid vertex buffer ID");
				return Failed;
			}

			index_buffer_object_t *iobj = getValidBuffer<index_buffer_object_t>(index_buffer_objects, ibid);
			if (iobj == NullPtr)
			{
				assert(false && "Invalid index buffer ID");
				return Failed;
			}

			if (iobj->ID == 0)
			{
				assert(false && "Invalid index buffer ID");
				return Failed;
			}

			GLuint dt = drawType == GY_DRAW_TRIANGLE ? GL_TRIANGLES : drawType == GY_DRAW_TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLE_FAN;
			uint32 sizeOfIndexType = iobj->SizeOfIndexElementType;
			uint32 sizeOfIndices = iobj->IndicesCount;
			GLuint es = sizeOfIndexType == 1 ? GL_UNSIGNED_BYTE : sizeOfIndexType == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iobj->ID);
			//glDrawElements(dt, sizeOfIndices, es, NullPtr);
			glDrawElementsInstancedBaseVertex(dt, sizeOfIndices, es, NullPtr, 1, 0);

			return Success;
		}
	}
}
#endif