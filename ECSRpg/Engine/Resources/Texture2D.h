#pragma once
#include <glad/glad.h>

struct Texture2D
{
	GLuint ID;
	GLuint Width;
	GLuint Height;

	GLuint Internal_Format;
	GLuint Image_Format;

	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;
	GLuint Filter_Max;

};

Texture2D ImportTextureFromFile(const char* file, GLboolean alpha);

