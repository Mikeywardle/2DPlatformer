#include "Vector2.h"

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2 Vector2::operator-(Vector2 b)
{
	return Vector2(
					x - b.x,
					y- b.y
				);
}

Vector2Int::Vector2Int(int x, int y)
{
	this->x = x;
	this->y = y;
}
