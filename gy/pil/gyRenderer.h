#ifndef _core_gyRenderer_h_
#define _core_gyRenderer_h_

#include "gyPlatformDetection.h"
#if defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
#	include <wrl/client.h>
#	include <agile.h>
#	include <d3d11_1.h>
#	include <ppl.h>
#	include <ppltasks.h>
#	include <DirectXMath.h>
#	include <memory>
#elif defined(__GY_OS_ENABLED_ANDROID__)
#	include <android/sensor.h>
#	include <android_native_app_glue.h>
#	include <android/log.h>
#	include <EGL/egl.h>
#	include <GLES/gl.h>
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#elif defined(__GY_OS_ENABLED_IOS__)
#elif defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
#	include <windows.h>
#elif defined(__GY_OS_ENABLED_MACOSX__)
#	define GLEW_STATIC
#	define GLEW_MX
#	include "glew/glew.h"
#	include <AGL/agl.h>
#elif defined(__GY_OS_ENABLED_LINUX__)
#	define GLEW_STATIC
#	define GLEW_MX
#	include "glew/glew.h"
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glx.h>
#endif
#if !defined(__GY_OS_ENABLED_WINSTORE__)
#	define STBI_NO_DDS
#	include "soil/SOIL.h" ///< for loading image resources
#endif
#include "gyMath.h"

namespace gy
{
	namespace type
	{
		enum renderer_platform_type_e
		{
			GY_RENDERER_UNKNOWN = -1,
			GY_RENDERER_AUTOMATIC = 0, ///< Win32/64, Linux, MacOSX는 OpenGL, iOS, Android는 OpenGLES, WinRT는 Direct3D를 선택한다.
			GY_RENDERER_OPENGL = 1,
			GY_RENDERER_OPENGLES = 2,
			GY_RENDERER_DIRECT3D = 3, ///< Only for Windows 8+ (in WinRT SDK)
			GY_RENDERER_TYPE_SIZE,
		};

		enum draw_type_e
		{
			GY_DRAW_TRIANGLE = 0,
			GY_DRAW_TRIANGLE_FAN,
			GY_DRAW_TRIANGLE_STRIP,
		};

		enum shader_type_e
		{
			GY_SHADER_UNKNOWN = -1,
			GY_SHADER_VERTEX = 0,
			GY_SHADER_GEOMETRY,
			GY_SHADER_FRAGMENT,
		};

		typedef id_t vertex_signature_t;
		extern vertex_signature_t GY_VS_UNKNOWN;
		extern vertex_signature_t GY_VS_POSITION;
		extern vertex_signature_t GY_VS_COLOR;
		extern vertex_signature_t GY_VS_NORMAL;
		extern vertex_signature_t GY_VS_DIFFUSE;
		extern vertex_signature_t GY_VS_TANGENT;
		extern vertex_signature_t GY_VS_BINORMAL;
		extern vertex_signature_t GY_VS_BLENDWEIGHTS;
		extern vertex_signature_t GY_VS_BLENDINDICES;
		extern vertex_signature_t GY_VS_TEXTURE0;
		extern vertex_signature_t GY_VS_TEXTURE1;
		extern vertex_signature_t GY_VS_TEXTURE2;
		extern vertex_signature_t GY_VS_TEXTURE3;
		extern vertex_signature_t GY_VS_TEXTURE4;
		extern vertex_signature_t GY_VS_TEXTURE5;
		extern vertex_signature_t GY_VS_TEXTURE6;
		extern vertex_signature_t GY_VS_TEXTURE7;

		typedef id_t vertex_signature_type_t;
		extern vertex_signature_type_t GY_VST_UNKNOWN;
		extern vertex_signature_type_t GY_VST_BYTE;
		extern vertex_signature_type_t GY_VST_UNSIGNED_BYTE;
		extern vertex_signature_type_t GY_VST_SHORT;
		extern vertex_signature_type_t GY_VST_UNSIGNED_SHORT;
		extern vertex_signature_type_t GY_VST_INT;
		extern vertex_signature_type_t GY_VST_UNSIGNED_INT;
		extern vertex_signature_type_t GY_VST_FLOAT;
		extern vertex_signature_type_t GY_VST_2_BYTES;
		extern vertex_signature_type_t GY_VST_3_BYTES;
		extern vertex_signature_type_t GY_VST_4_BYTES;
		extern vertex_signature_type_t GY_VST_DOUBLE;
		extern vertex_signature_type_t GY_VST_FLOAT_VEC2;
		extern vertex_signature_type_t GY_VST_FLOAT_VEC3;
		extern vertex_signature_type_t GY_VST_FLOAT_VEC4;
		extern vertex_signature_type_t GY_VST_INT_VEC2;
		extern vertex_signature_type_t GY_VST_INT_VEC3;
		extern vertex_signature_type_t GY_VST_INT_VEC4;
		extern vertex_signature_type_t GY_VST_BOOL;
		extern vertex_signature_type_t GY_VST_BOOL_VEC2;
		extern vertex_signature_type_t GY_VST_BOOL_VEC3;
		extern vertex_signature_type_t GY_VST_BOOL_VEC4;
		extern vertex_signature_type_t GY_VST_FLOAT_MAT2;
		extern vertex_signature_type_t GY_VST_FLOAT_MAT3;
		extern vertex_signature_type_t GY_VST_FLOAT_MAT4;
		extern vertex_signature_type_t GY_VST_SAMPLER_1D;
		extern vertex_signature_type_t GY_VST_SAMPLER_2D;
		extern vertex_signature_type_t GY_VST_SAMPLER_3D;
		extern vertex_signature_type_t GY_VST_SAMPLER_CUBE;
		extern vertex_signature_type_t GY_VST_SAMPLER_1D_SHADOW;
		extern vertex_signature_type_t GY_VST_SAMPLER_2D_SHADOW;

		/// vertex_layout_t has signature and format per vertex
		struct vertex_layout_t
		{
			enum {MAX_SIGN_LENGTH = 256};
			char Signature[MAX_SIGN_LENGTH];
			vertex_signature_t SignID;
			vertex_signature_type_t Type;
			uint32 Size;
		};
		extern vertex_layout_t END_OF_VERTEX_LAYOUT;

		typedef id_t renderer_t;
		const renderer_t GY_INVALID_RENDERER_ID = Invalid;
		typedef id_t vertex_buffer_t;
		const vertex_buffer_t GY_INVALID_VERTEX_BUFFER_ID = Invalid;
		typedef id_t vertex_format_t;
		const vertex_format_t GY_INVALID_VERTEX_FORMAT_ID = Invalid;
		typedef id_t index_buffer_t;
		const index_buffer_t GY_INVALID_INDEX_BUFFER_ID = Invalid;
		typedef id_t texture_t;
		const texture_t GY_INVALID_TEXTURE_ID = Invalid;
		typedef id_t program_t;
		const program_t GY_INVALID_PROGRAM_ID = Invalid;
		typedef id_t shader_t;
		const shader_t GY_INVALID_SHADER_ID = Invalid;
#if defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
		float DX_ConvertDipsToPixels(float dips);
		void DX_ThrowIfFailed(HRESULT hr);
		// Function that reads from a binary file asynchronously.
		Concurrency::task<Platform::Array<byte>^> DX_ReadDataAsync(Platform::String^ filename);
		struct _renderer_object_t
		{
			// Direct3D Objects.
			Microsoft::WRL::ComPtr<ID3D11Device1> Device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext1> Context;
			Microsoft::WRL::ComPtr<IDXGISwapChain1> SwapChain;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
			// Cached renderer properties.
			D3D_FEATURE_LEVEL FeatureLevel;
			Windows::Foundation::Size RenderTargetSize;
			Windows::Foundation::Rect WindowBounds;
			Platform::Agile<Windows::UI::Core::CoreWindow> Window;
			Windows::Graphics::Display::DisplayOrientations Orientation;
			DirectX::XMFLOAT4X4 OrientationTransform3D;
		};
		typedef _renderer_object_t *renderer_object_t;

#elif defined(__GY_OS_ENABLED_ANDROID__)
		struct _renderer_object_t
		{
			EGLDisplay Display;
			EGLContext Context;
			EGLSurface Surface;
			EGLConfig Config;
			const char *Extensions;
			ANativeWindow *Window;
		};
		typedef _renderer_object_t *renderer_object_t;
#elif defined(__GY_OS_ENABLED_IOS__)
		struct _renderer_object_t
		{
			void *Renderer;
			void *Window;
		};
		typedef _renderer_object_t *renderer_object_t;
#elif defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
		struct _renderer_object_t
		{
			HGLRC RendererContext;
			HDC DeviceContext;
			HWND Window;
		};
		typedef _renderer_object_t *renderer_object_t;
	
#elif defined(__GY_OS_ENABLED_MACOSX__)
		//typedef AGLContext renderer_object_t;
		//typedef EAGLContext renderer_object_t;
		//typedef void *renderer_object_t;
		struct _renderer_object_t
		{
			void *Renderer;
			void *Window;
			GLEWContext GLEWContext_;
		};
		typedef _renderer_object_t *renderer_object_t;
#elif defined(__GY_OS_ENABLED_LINUX__)
		struct _renderer_object_t
		{
			GLXContext Renderer;
			Window Window_;
			GLEWContext GLEWContext_;
		};
		typedef _renderer_object_t *renderer_object_t;
#else
#endif
	}
	using type::draw_type_e;
	using type::renderer_platform_type_e;
	using type::renderer_t;
	using type::renderer_object_t;
	using type::vertex_buffer_t;
	using type::index_buffer_t;
	using type::shader_t;
	using type::texture_t;
	using type::shader_type_e;
	using type::vertex_format_t;

	// OS특화 도움 함수들 선언
#if defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
#elif defined(__GY_OS_ENABLED_MACOSX__)
#elif defined(__GY_OS_ENABLED_LINUX__)
	// X11연관 함수들
	Window getWindowFromRenderer(renderer_object_t rid);
#endif

	/// (r3: (r)ende(r)e(r)의 모든 r을 의미)
	namespace r3
	{
		int initializeRenderer(renderer_platform_type_e rendererPlatform = GY_RENDERER_AUTOMATIC);
		int getRendererAPIType();
		uint32 getMaxAllocableRendererObjectCount();
		bool isValidRenderer();
		bool isValidRenderer(renderer_t rid);
		int setCurrentRendererID(renderer_t rid);
		renderer_t getCurrentRendererID();
		renderer_object_t getRendererObject(renderer_t rid);

		// OS별 특징이 있는 공통 함수들
		/// Renderer생성. 생성한 Renderer의 고유한 ID를 반환한다.
		renderer_t createRenderer(void *vid, size_t width = _Default, size_t height = _Default);
		/// Renderer생성. 생성한 Renderer의 고유한 ID를 반환한다.
		//renderer_t createRenderer(const _renderer_object_t &rawRendererContext);
		int destroyRenderer(renderer_t rid);
		uint getScreenWidth();
		uint getScreenHeight();

		// 스크린 버퍼 클리어 및 플러싱
		uint32 getClearColor();
		int setClearColor(const uint32 &color); 
		int clear();
		int begin();
		int end();
		int flush();

		// 버텍스 버퍼
		vertex_format_t createVertexFormat(const vertex_layout_t *vertexLayoutArray, uint32 dataSize);
		vertex_buffer_t createVertexBuffer(const void *vertexBuffer, uint32 dataSize, const vertex_format_t vertexFormatID, int usage);
		const char *getStringOfVertexSignature(vertex_signature_type_t vs);
		size_t getSizeOfVertexSignature(vertex_signature_type_t vs);
		size_t getSizeOfVertexFormat(vertex_format_t vfid);

		// 인덱스 버퍼
		index_buffer_t createIndexBuffer(uint32 sizeOfElementType, uint32 elementCount, const void *indexBuffer, int usage);

		// 텍스쳐 버퍼
		texture_t createTextureFromFile(const char *fileName, uint32 width = _Default, uint32 height = _Default);
		texture_t createTextureFromMemory(void *rawBuffer, uint32 width = _Default, uint height = _Default);
		int bindTexture(texture_t tid);
		int destroyTexture(texture_t tid);

		// 셰이더
		program_t createShaderProgram();
		int linkShaderProgram(program_t pid);
		shader_t createShaderFromFile(program_t pid, shader_type_e shaderProgramType, const char *fileName);
		shader_t createShaderFromMemory(program_t pid, shader_type_e shaderProgramType, const char *buffer);
		int beginShader(program_t pid);
		int endShader();
		// setting uniform variables when shaders has linked time.
		int setShaderVariableFloat(const char *variableName, float v0);
		int setShaderVariableFloat(const char *variableName, float v0, float v1);
		int setShaderVariableFloat(const char *variableName, float v0, float v1, float v2);
		int setShaderVariableFloat(const char *variableName, float v0, float v1, float v2, float v3);
		int setShaderVariableVector(const char *variableName, const gy::math::vec2 &v);
		int setShaderVariableVector(const char *variableName, const gy::math::vec3 &v);
		int setShaderVariableVector(const char *variableName, const gy::math::vec4 &v);
		int setShaderVariableMatrix(const char *variableName, const gy::math::mat2x2 &v);
		int setShaderVariableMatrix(const char *variableName, const gy::math::mat3x3 &v);
		int setShaderVariableMatrix(const char *variableName, const gy::math::mat4x4 &v);
		// below functions are for didn't preload uniform variables when shaders has linked time.
		int setShaderVariableFloat(program_t pid, const char *variableName, float v0);
		int setShaderVariableFloat(program_t pid, const char *variableName, float v0, float v1);
		int setShaderVariableFloat(program_t pid, const char *variableName, float v0, float v1, float v2);
		int setShaderVariableFloat(program_t pid, const char *variableName, float v0, float v1, float v2, float v3);
		int setShaderVariableVector(program_t pid, const char *variableName, const gy::math::vec2 &v);
		int setShaderVariableVector(program_t pid, const char *variableName, const gy::math::vec3 &v);
		int setShaderVariableVector(program_t pid, const char *variableName, const gy::math::vec4 &v);
		int setShaderVariableMatrix(program_t pid, const char *variableName, const gy::math::mat2x2 &v);
		int setShaderVariableMatrix(program_t pid, const char *variableName, const gy::math::mat3x3 &v);
		int setShaderVariableMatrix(program_t pid, const char *variableName, const gy::math::mat4x4 &v);

		/*
		OpenGL (http://www.lighthouse3d.com/tutorials/glsl-core-tutorial/attribute-variables/)
		#version 330

		in vec3 position;
		in vec3 normal;
		in vec2 texCoord;
		...

		// Create shader program.
		_program = glCreateProgram();

		// Compile
		*shader = glCreateShader(type);
		glShaderSource(*shader, 1, &source, NULL);
		glCompileShader(*shader);

		#if defined(DEBUG)
		GLint logLength;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
		GLchar *log = (GLchar *)malloc(logLength);
		glGetShaderInfoLog(*shader, logLength, &logLength, log);
		NSLog(@"Shader compile log:\n%s", log);
		free(log);
		}
		#endif

		glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
		if (status == 0) {
		glDeleteShader(*shader);
		return NO;
		}
		// Attach vertex shader to program.
		glAttachShader(_program, vertShader);

		// Attach fragment shader to program.
		glAttachShader(_program, fragShader);

		// Bind attribute locations.
		// This needs to be done prior to linking.
		glBindAttribLocation(_program, GLKVertexAttribPosition, "position");
		glBindAttribLocation(_program, GLKVertexAttribNormal, "normal");

		// Link
		glLinkProgram(prog);

		#if defined(DEBUG)
		GLint logLength;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
		GLchar *log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(prog, logLength, &logLength, log);
		NSLog(@"Program link log:\n%s", log);
		free(log);
		}
		#endif

		glGetProgramiv(prog, GL_LINK_STATUS, &status);

		// Get uniform locations.
		uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
		uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");

		// Release vertex and fragment shaders.
		if (vertShader) {
		glDetachShader(_program, vertShader);
		glDeleteShader(vertShader);
		}
		if (fragShader) {
		glDetachShader(_program, fragShader);
		glDeleteShader(fragShader);
		}
		*/

		// 그리기
		int draw(draw_type_e drawType, vertex_buffer_t vbid);
		int drawIndexed(draw_type_e drawType, vertex_buffer_t vbid, index_buffer_t ibid);
	}

	using namespace r3;
}
using gy::renderer_object_t;

#if defined(__GY_OS_ENABLED_WINDOWS__)
#elif defined(__GY_OS_ENABLED_MACOSX__)
#elif defined(__GY_OS_ENABLED_LINUX__)
#endif

#endif