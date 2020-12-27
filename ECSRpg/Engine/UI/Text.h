#pragma once

#include <Maths/Vector2.h>
#include <ft2build.h>
#include <Maths/Color.h>
#include FT_FREETYPE_H 

#include <string>

typedef struct
{
	unsigned int TextureID;
	Vector2Int Size;
	Vector2Int Bearing;
	unsigned int Advance;
} Character;

void InitializeTextData();
void RenderText(std::string string, Vector2 position, Color color);