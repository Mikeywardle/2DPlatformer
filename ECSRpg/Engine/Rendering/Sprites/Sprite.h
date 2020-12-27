#pragma once
#include <Maths/Vector3.h>
#include <Maths/Color.h>
#include <Resources/Texture2D.h>

struct SpriteComponent
{
	SpriteComponent() = default;
	SpriteComponent(Texture2D* texture, Color color, float PixelsPerUnit);

	float GetWidth() const;
	float GetHeight() const;
	int GetTextureID() const;

	Color color;
	float PixelsPerUnit;

private:
	int spriteTexture;
	float Width;
	float Height;

};