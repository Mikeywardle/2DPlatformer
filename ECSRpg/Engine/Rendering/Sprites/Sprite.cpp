#include "Sprite.h"

SpriteComponent::SpriteComponent(Texture2D* texture, Color color, float PixelsPerUnit)
{
	this->spriteTexture = texture->ID;
	this->color = color;
	this->Width = texture->Width;
	this->Height = texture->Height;
	this->PixelsPerUnit = PixelsPerUnit;
}

float SpriteComponent::GetWidth() const
{
	return Width;
}

float SpriteComponent::GetHeight() const
{
	return Height;
}

int SpriteComponent::GetTextureID() const
{
	return spriteTexture;
}
