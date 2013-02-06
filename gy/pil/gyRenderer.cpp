#include "gyRenderer.h"
#if !defined(__GY_OS_ENABLED_WINSTORE__) && !defined(__GY_OS_ENABLED_WINPHONE__)
extern "C"
{
#	include "soil/SOIL.c"
#	include "soil/image_DXT.c"
#	include "soil/stb_image_aug.c"
#	include "soil/image_helper.c"
}
#endif
namespace gy
{
	namespace type
	{
		struct __vertex_signature_t
		{
		private:
			__vertex_signature_t() {}

		public:
			enum
			{
				GY_VS_UNKNOWN = -1,
				GY_VS_POSITION,
				GY_VS_COLOR,
				GY_VS_NORMAL,
				GY_VS_DIFFUSE,
				GY_VS_TANGENT,
				GY_VS_BINORMAL,
				GY_VS_BLENDWEIGHTS,
				GY_VS_BLENDINDICES,
				GY_VS_TEXTURE0,
				GY_VS_TEXTURE1,
				GY_VS_TEXTURE2,
				GY_VS_TEXTURE3,
				GY_VS_TEXTURE4,
				GY_VS_TEXTURE5,
				GY_VS_TEXTURE6,
				GY_VS_TEXTURE7,
			};
		};

		struct __vertex_signature_type_t
		{
private:
			__vertex_signature_type_t() {}

		public:
			enum
			{
				GY_VST_UNKNOWN = -1,
				GY_VST_BYTE,
				GY_VST_UNSIGNED_BYTE,
				GY_VST_SHORT,
				GY_VST_UNSIGNED_SHORT,
				GY_VST_INT,
				GY_VST_UNSIGNED_INT,
				GY_VST_FLOAT,
				GY_VST_2_BYTES,
				GY_VST_3_BYTES,
				GY_VST_4_BYTES,
				GY_VST_DOUBLE,
				GY_VST_FLOAT_VEC2,
				GY_VST_FLOAT_VEC3,
				GY_VST_FLOAT_VEC4,
				GY_VST_INT_VEC2,
				GY_VST_INT_VEC3,
				GY_VST_INT_VEC4,
				GY_VST_BOOL,
				GY_VST_BOOL_VEC2,
				GY_VST_BOOL_VEC3,
				GY_VST_BOOL_VEC4,
				GY_VST_FLOAT_MAT2,
				GY_VST_FLOAT_MAT3,
				GY_VST_FLOAT_MAT4,
				GY_VST_SAMPLER_1D,
				GY_VST_SAMPLER_2D,
				GY_VST_SAMPLER_3D,
				GY_VST_SAMPLER_CUBE,
				GY_VST_SAMPLER_1D_SHADOW,
				GY_VST_SAMPLER_2D_SHADOW,
			};
		};

		vertex_signature_t GY_VS_UNKNOWN = __vertex_signature_t::GY_VS_UNKNOWN;
		vertex_signature_t GY_VS_POSITION = __vertex_signature_t::GY_VS_POSITION;
		vertex_signature_t GY_VS_COLOR = __vertex_signature_t::GY_VS_COLOR;
		vertex_signature_t GY_VS_NORMAL = __vertex_signature_t::GY_VS_NORMAL;
		vertex_signature_t GY_VS_DIFFUSE = __vertex_signature_t::GY_VS_DIFFUSE;
		vertex_signature_t GY_VS_TANGENT = __vertex_signature_t::GY_VS_TANGENT;
		vertex_signature_t GY_VS_BINORMAL = __vertex_signature_t::GY_VS_BINORMAL;
		vertex_signature_t GY_VS_BLENDWEIGHTS = __vertex_signature_t::GY_VS_BLENDWEIGHTS;
		vertex_signature_t GY_VS_BLENDINDICES = __vertex_signature_t::GY_VS_BLENDINDICES;
		vertex_signature_t GY_VS_TEXTURE0 = __vertex_signature_t::GY_VS_TEXTURE0;
		vertex_signature_t GY_VS_TEXTURE1 = __vertex_signature_t::GY_VS_TEXTURE1;
		vertex_signature_t GY_VS_TEXTURE2 = __vertex_signature_t::GY_VS_TEXTURE2;
		vertex_signature_t GY_VS_TEXTURE3 = __vertex_signature_t::GY_VS_TEXTURE3;
		vertex_signature_t GY_VS_TEXTURE4 = __vertex_signature_t::GY_VS_TEXTURE4;
		vertex_signature_t GY_VS_TEXTURE5 = __vertex_signature_t::GY_VS_TEXTURE5;
		vertex_signature_t GY_VS_TEXTURE6 = __vertex_signature_t::GY_VS_TEXTURE6;
		vertex_signature_t GY_VS_TEXTURE7 = __vertex_signature_t::GY_VS_TEXTURE7;

		/*
		GL_BYTE 0x1400
		GL_UNSIGNED_BYTE 0x1401
		GL_SHORT 0x1402
		GL_UNSIGNED_SHORT 0x1403
		GL_INT 0x1404
		GL_UNSIGNED_INT 0x1405
		GL_FLOAT 0x1406
		GL_2_BYTES 0x1407
		GL_3_BYTES 0x1408
		GL_4_BYTES 0x1409
		GL_DOUBLE 0x140A
		GL_FLOAT_VEC2 0x8B50
		GL_FLOAT_VEC3 0x8B51
		GL_FLOAT_VEC4 0x8B52
		GL_INT_VEC2 0x8B53
		GL_INT_VEC3 0x8B54
		GL_INT_VEC4 0x8B55
		GL_BOOL 0x8B56
		GL_BOOL_VEC2 0x8B57
		GL_BOOL_VEC3 0x8B58
		GL_BOOL_VEC4 0x8B59
		GL_FLOAT_MAT2 0x8B5A
		GL_FLOAT_MAT3 0x8B5B
		GL_FLOAT_MAT4 0x8B5C
		GL_SAMPLER_1D 0x8B5D
		GL_SAMPLER_2D 0x8B5E
		GL_SAMPLER_3D 0x8B5F
		GL_SAMPLER_CUBE 0x8B60
		GL_SAMPLER_1D_SHADOW 0x8B61
		GL_SAMPLER_2D_SHADOW 0x8B62
		*/
		vertex_signature_type_t GY_VST_UNKNOWN = __vertex_signature_type_t::GY_VST_UNKNOWN;
		vertex_signature_type_t GY_VST_BYTE = __vertex_signature_type_t::GY_VST_BYTE;
		vertex_signature_type_t GY_VST_UNSIGNED_BYTE = __vertex_signature_type_t::GY_VST_UNSIGNED_BYTE;
		vertex_signature_type_t GY_VST_SHORT = __vertex_signature_type_t::GY_VST_SHORT;
		vertex_signature_type_t GY_VST_UNSIGNED_SHORT = __vertex_signature_type_t::GY_VST_UNSIGNED_SHORT;
		vertex_signature_type_t GY_VST_INT = __vertex_signature_type_t::GY_VST_INT;
		vertex_signature_type_t GY_VST_UNSIGNED_INT = __vertex_signature_type_t::GY_VST_UNSIGNED_INT;
		vertex_signature_type_t GY_VST_FLOAT = __vertex_signature_type_t::GY_VST_FLOAT;
		vertex_signature_type_t GY_VST_2_BYTES = __vertex_signature_type_t::GY_VST_2_BYTES;
		vertex_signature_type_t GY_VST_3_BYTES = __vertex_signature_type_t::GY_VST_3_BYTES;
		vertex_signature_type_t GY_VST_4_BYTES = __vertex_signature_type_t::GY_VST_4_BYTES;
		vertex_signature_type_t GY_VST_DOUBLE = __vertex_signature_type_t::GY_VST_DOUBLE;
		vertex_signature_type_t GY_VST_FLOAT_VEC2 = __vertex_signature_type_t::GY_VST_FLOAT_VEC2;
		vertex_signature_type_t GY_VST_FLOAT_VEC3 = __vertex_signature_type_t::GY_VST_FLOAT_VEC3;
		vertex_signature_type_t GY_VST_FLOAT_VEC4 = __vertex_signature_type_t::GY_VST_FLOAT_VEC4;
		vertex_signature_type_t GY_VST_INT_VEC2 = __vertex_signature_type_t::GY_VST_INT_VEC2;
		vertex_signature_type_t GY_VST_INT_VEC3 = __vertex_signature_type_t::GY_VST_INT_VEC3;
		vertex_signature_type_t GY_VST_INT_VEC4 = __vertex_signature_type_t::GY_VST_INT_VEC4;
		vertex_signature_type_t GY_VST_BOOL = __vertex_signature_type_t::GY_VST_BOOL;
		vertex_signature_type_t GY_VST_BOOL_VEC2 = __vertex_signature_type_t::GY_VST_BOOL_VEC2;
		vertex_signature_type_t GY_VST_BOOL_VEC3 = __vertex_signature_type_t::GY_VST_BOOL_VEC3;
		vertex_signature_type_t GY_VST_BOOL_VEC4 = __vertex_signature_type_t::GY_VST_BOOL_VEC4;
		vertex_signature_type_t GY_VST_FLOAT_MAT2 = __vertex_signature_type_t::GY_VST_FLOAT_MAT2;
		vertex_signature_type_t GY_VST_FLOAT_MAT3 = __vertex_signature_type_t::GY_VST_FLOAT_MAT3;
		vertex_signature_type_t GY_VST_FLOAT_MAT4 = __vertex_signature_type_t::GY_VST_FLOAT_MAT4;
		vertex_signature_type_t GY_VST_SAMPLER_1D = __vertex_signature_type_t::GY_VST_SAMPLER_1D;
		vertex_signature_type_t GY_VST_SAMPLER_2D = __vertex_signature_type_t::GY_VST_SAMPLER_2D;
		vertex_signature_type_t GY_VST_SAMPLER_3D = __vertex_signature_type_t::GY_VST_SAMPLER_3D;
		vertex_signature_type_t GY_VST_SAMPLER_CUBE = __vertex_signature_type_t::GY_VST_SAMPLER_CUBE;
		vertex_signature_type_t GY_VST_SAMPLER_1D_SHADOW = __vertex_signature_type_t::GY_VST_SAMPLER_1D_SHADOW;
		vertex_signature_type_t GY_VST_SAMPLER_2D_SHADOW = __vertex_signature_type_t::GY_VST_SAMPLER_2D_SHADOW;

		vertex_layout_t END_OF_VERTEX_LAYOUT;
	}

	namespace r3
	{
		int renderer_platform_id = GY_RENDERER_UNKNOWN;
		int initializeRenderer(renderer_platform_type_e rendererPlatform)
		{
			if (renderer_platform_id != GY_RENDERER_UNKNOWN)
			{
				// 이미 초기화되어있음.
				assert(false);
				return Failed;
			}

			if (rendererPlatform >= GY_RENDERER_TYPE_SIZE)
			{
				rendererPlatform = GY_RENDERER_UNKNOWN;
				return Failed;
			}

			if (rendererPlatform <= GY_RENDERER_UNKNOWN)
			{
				rendererPlatform = GY_RENDERER_UNKNOWN;
				return Failed;
			}

			if (rendererPlatform == GY_RENDERER_AUTOMATIC)
			{
#if defined(__GY_OS_ENABLED_ANDROID__)
				rendererPlatform = GY_RENDERER_OPENGLES;
#elif defined(__GY_OS_ENABLED_IOS__)
				rendererPlatform = GY_RENDERER_OPENGLES;
#elif defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
				rendererPlatform = GY_RENDERER_OPENGL;
#elif defined(__GY_OS_ENABLED_WINSTORE__)
				rendererPlatform = GY_RENDERER_DIRECT3D;
#elif defined(__GY_OS_ENABLED_MACOSX__)
				rendererPlatform = GY_RENDERER_OPENGL;
#elif defined(__GY_OS_ENABLED_LINUX__)
				rendererPlatform = GY_RENDERER_OPENGL;
#endif
			}

			renderer_platform_id = rendererPlatform;
			
			return Success;
		}

		int getRendererAPIType()
		{
			return renderer_platform_id;
		}
	}
}