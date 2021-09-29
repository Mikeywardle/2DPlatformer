#include "Vector2.h"

#include <math.h>
#include <Maths/MathsHelpers.h>

const Vector2 Vector2::Up = { 0,1 };
const Vector2 Vector2::Down = { 0,-1 };
const Vector2 Vector2::Left = { -1,0 };
const Vector2 Vector2::Right = { 1,0 };
const Vector2 Vector2::Zero = { 0,0 };
const Vector2 Vector2::One = { 1,1 };

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2 Vector2::operator+(Vector2 b) const
{
	return Vector2
	(
		x + b.x,
		y + b.y
	);
}

Vector2 Vector2::operator-(Vector2 b) const
{
	return Vector2
	(
		x - b.x,
		y - b.y
	);
}

Vector2 Vector2::operator*(Vector2 b) const
{
	return Vector2
	(
		x * b.x,
		y * b.y
	);
}

Vector2 Vector2::operator/(Vector2 b) const
{
	return Vector2
	(
		x / b.x,
		y / b.y
	);
}

void Vector2::operator+=(Vector2 b)
{
	x += b.x;
	y += b.y;
}

void Vector2::operator-=(Vector2 b)
{
	x -= b.x;
	y -= b.y;
}

void Vector2::operator*=(Vector2 b)
{
	x *= b.x;
	y *= b.y;
}

void Vector2::operator/=(Vector2 b)
{
	x /= b.x;
	y /= b.y;
}

Vector2 Vector2::operator*(float b) const
{
	return Vector2
	(
		x * b,
		y * b
	);
}

Vector2 Vector2::operator/(float b) const
{
	return Vector2
	(
		x / b,
		y / b
	);
}

void Vector2::operator*=(float b)
{
	x *= b;
	y *= b;
}

float Vector2::Length() const
{
	return sqrtf((x * x) + (y * y));
}

Vector2 Vector2::Normalize()
{
	float length = this->Length();
	if (length == 0)
		return *this;

	return *this / length;
}

Vector2 Vector2::Abs()
{
	return Vector2
	(
		fabsf(x)
		, fabsf(y)
	);
}

Vector2 Vector2::Clamp(const Vector2 Max, const Vector2 Min)
{
	return Vector2
	(
		ClampValue(x, Max.x, Min.x)
		, ClampValue(y, Max.y, Min.y)
	);
}

float Vector2::DotProduct(Vector2 a, Vector2 b)
{
	return (a.x * b.x) + (a.y * b.y);
}


Vector2Int::Vector2Int(int x, int y)
{
	this->x = x;
	this->y = y;
}
