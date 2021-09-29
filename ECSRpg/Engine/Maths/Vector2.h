#pragma once

struct Vector2
{
public:
	Vector2() = default;
	Vector2(float x, float y);

public:
	float x = 0;
	float y = 0;


	static const Vector2 Up;
	static const Vector2 Down;
	static const Vector2 Left;
	static const Vector2 Right;
	static const Vector2 Zero;
	static const Vector2 One;

public:
	Vector2 operator + (Vector2 b) const;
	Vector2 operator - (Vector2 b) const;
	Vector2 operator * (Vector2 b) const;
	Vector2 operator / (Vector2 b) const;

	void operator += (Vector2 b);
	void operator -= (Vector2 b);
	void operator *= (Vector2 b);
	void operator /= (Vector2 b);

	Vector2 operator * (float b) const;
	Vector2 operator / (float b) const;

	void operator *= (float b);

	float Length() const;

	Vector2 Normalize();
	Vector2 Abs();
	Vector2 Clamp(const Vector2 Max, const Vector2 Min);

	static float DotProduct(Vector2 a, Vector2 b);

};

struct Vector2Int
{
	Vector2Int() {}
	Vector2Int(int x, int y);

	int x = 0;
	int y = 0;
};

