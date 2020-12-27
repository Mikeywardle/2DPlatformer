#pragma once

struct Vector2
{
	Vector2() {}
	Vector2(float x, float y);

	float x = 0;
	float y = 0;

	Vector2 operator - (Vector2 b);
};

struct Vector2Int
{
	Vector2Int() {}
	Vector2Int(int x, int y);

	int x = 0;
	int y = 0;
};

