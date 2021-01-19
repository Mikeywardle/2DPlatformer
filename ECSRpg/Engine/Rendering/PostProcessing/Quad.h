#pragma once

#include <Maths/Vector3.h>
#include <Maths/Vector2.h>

struct QuadVertex
{
	Vector2 Position;
	Vector2 TexCoords;
};

struct PostProcessQuad
{
	PostProcessQuad();

	void Draw();

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	QuadVertex vertices[4] = 
	{ 
		{Vector2(-1,-1),Vector2(0,0)},
		{Vector2(1,-1),Vector2(1,0)},
		{Vector2(1,1),Vector2(1,1)},
		{Vector2(-1,1),Vector2(0,1)} 
	};

	unsigned int indices[6] = {0,1,2,2,3,0};
};
