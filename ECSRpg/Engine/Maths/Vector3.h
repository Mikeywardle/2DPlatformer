#pragma once

struct Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;

	Vector3() {};
	Vector3(float x, float y, float z);

	//Vector3 Operators
	Vector3 operator+ (const Vector3 b) const;
	Vector3 operator- (const Vector3 b) const;
	Vector3 operator/ (const Vector3 b) const;
	Vector3 operator* (const Vector3 b) const;

	void operator += (const Vector3 b);
	void operator -= (const Vector3 b);
	void operator /= (const Vector3 b);
	void operator *= (const Vector3 b);

	//Float operators
	Vector3 operator+ (const float b) const;
	Vector3 operator- (const float b) const;
	Vector3 operator/ (const float b) const;
	Vector3 operator* (const float b) const;

	void operator += (const float b);
	void operator -= (const float b);
	void operator /= (const float b);
	void operator *= (const float b);

	//Static methods
	static Vector3 CrossProduct(const Vector3 a, const Vector3 b);
	static float DotProduct(const Vector3 a, const Vector3 b);
	static Vector3 Normalize(Vector3 a);
	static float Magnitude(Vector3 a);
	static Vector3 Lerp(Vector3 a, Vector3 b, float alpha);
	static float ReverseLerp(Vector3 a, Vector3 b, Vector3 output);

	Vector3 Clamp(const Vector3 Max, const Vector3 Min) const;

	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Right;
	static const Vector3 Left;
	static const Vector3 Forwards;
	static const Vector3 Backwards;

	static const Vector3 Zero;
	static const Vector3 One;
};


