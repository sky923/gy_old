//
//  main.cpp
//  gy
//
//  Created by gil on 9/8/12.
//  Copyright (c) 2012 sky923. All rights reserved.
//
#include <iostream>
#include <gy/gy.h>
The_Main Is_Here;

using namespace gy;
using namespace gy::math;

float gCubeVertexData[] = 
{
	// Data layout for each line below is:
	// positionX, positionY, positionZ,     normalX, normalY, normalZ,
	0.5f, -0.5f, -0.5f, 1.0f,       1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f,        1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f,        1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f,        1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f,         1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f,        1.0f, 0.0f, 0.0f,

	0.5f, 0.5f, -0.5f, 1.0f,        0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f,       0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f,         0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f,         0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f,       0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f,        0.0f, 1.0f, 0.0f,

	-0.5f, 0.5f, -0.5f, 1.0f,       -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,      -1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f,        -1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f,        -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,      -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 1.0f,       -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 1.0f,      0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f,       0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 1.0f,       0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 1.0f,       0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f,       0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f,        0.0f, -1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f,         0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 1.0f,        0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f,        0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f,        0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 1.0f,        0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 1.0f,       0.0f, 0.0f, 1.0f,

	0.5f, -0.5f, -0.5f, 1.0f,       0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,      0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f, 1.0f,        0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f, 1.0f,        0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,      0.0f, 0.0f, -1.0f,
	-0.5f, 0.5f, -0.5f, 1.0f,       0.0f, 0.0f, -1.0f
};

class CTest01 : public gyMain
{
private:
	vertex_buffer_t vb;
	vertex_format_t vf;
	program_t p;
	shader_t vs;
	shader_t fs;
public:
    
	virtual int onPrepare()
	{
        /*
		vb = GY_INVALID_VERTEX_BUFFER_ID;

		setClearColor(0xFF113355);

		// 어플리케이션 사용자화 부분
		uint32 u32 = 1234343434;
		c3u16 c(u32, 1000, 1000);
		std::cout << c.r << ", " << c.g << ", " << c.b << std::endl;

		c3f cf(c3f_100);
		std::cout << cf.r << ", " << cf.g << ", " << cf.b << std::endl;

		std::cout << "Hello World! I'm " << __GY_ID_STR_OS__ << "!" << std::endl;

		for (int i = 0; i < getArgumentSize(); ++i)
			std::cout << "ARG (" << i << ") : " << getArgument(i) << std::endl;

		std::cout << std::endl;

		float fov = 2 * math::fastAtan(35.0f * 1.5f / (2.0f * 250.0f));
		std::cout << "fov = " << fov << std::endl;

		vertex_layout_t layout[] =
		{
			//-------------------------------------------------------//
			{"position",	GY_VS_POSITION,		GY_VST_FLOAT_VEC4,	1},
			{"normal",		GY_VS_NORMAL,		GY_VST_FLOAT_VEC3,	1},
			//-------------------------------------------------------//
			//END_OF_VERTEX_LAYOUT
		};

		const char *varyingBuffer =
			"\
			struct INPUT\
			{\
			vec3 position : POSITION;\
			vec3 normal : NORMAL;\
			vec3 color : COLOR0;\
			};\
			\
			varying struct OUTPUT\
			{\
			vec3 position : POSITION;\
			vec3 color : COLOR0;\
			};\
			\
			";

#ifndef __GY_OS_ENABLED_WINAPP__
		vf = createVertexFormat(layout, sizeof(layout));
		p = createShaderProgram();
		vs = createShaderFromFile(p, GY_SHADER_VERTEX, "../../GLES2_Shader.vsh");
		fs = createShaderFromFile(p, GY_SHADER_FRAGMENT, "../../GLES2_Shader.fsh");
		vb = createVertexBuffer(gCubeVertexData, sizeof(gCubeVertexData), vf, 0);
#endif
*/
		return Success;
	}

	float _rotation;
	mat3x3 _normalMatrix;
	mat4x4 _modelViewProjectionMatrix;
    /*
	virtual int onUpdate()
	{
		renderer_t rid = gy::getCurrentRendererID();
		if (rid == GY_INVALID_RENDERER_ID)
			return Failed;

		float screenWidth = (float)getScreenWidth();
		float screenHeight = (float)getScreenHeight();
		float aspect = screenWidth / screenHeight;
		mat4x4 projectionMatrix = perspective(200.0f, aspect, 0.1f, 100.0f);
		mat4x4 baseModelViewMatrix = translate(0.0f, 0.0f, -4.0f);

		// Compute the model view matrix for the object rendered with GLKit
		mat4x4 modelViewMatrix = translate(0.0f, 0.0f, -1.5f);
		modelViewMatrix = rotate(modelViewMatrix, _rotation, 1.0f, 1.0f, 1.0f);
		modelViewMatrix = baseModelViewMatrix * modelViewMatrix;

		// Compute the model view matrix for the object rendered with ES2
		modelViewMatrix = translate(0.0f, 0.0f, 1.5f);
		modelViewMatrix = rotate(modelViewMatrix, _rotation, 1.0f, 1.0f, 1.0f);
		modelViewMatrix = baseModelViewMatrix * modelViewMatrix;

		_normalMatrix = (mat3x3)(transpose(inverse(modelViewMatrix)));
		_modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
		_rotation = (float)GetTickCount() / 75.0f;

		return Success;
	}

	virtual int onRender()
	{
		clear();
		{
			beginShader(p);
			{
				//@todo: 정육면체가 제데로 나오는 구현체 완성하기
				setShaderVariableMatrix("modelViewProjectionMatrix", _modelViewProjectionMatrix);
				setShaderVariableMatrix("normalMatrix", _normalMatrix);
				draw(GY_DRAW_TRIANGLE, vb);
			}
			endShader();
		}
		flush();

		return Success;
	}
    */
} MainObject;
