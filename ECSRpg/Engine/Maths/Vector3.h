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

};

static Vector3 VECTOR3_UP = { 0,1,0 };
static Vector3 VECTOR3_DOWN = { 0,-1,0 };
static Vector3 VECTOR3_RIGHT = { -1,0,0 };
static Vector3 VECTOR3_LEFT = { 1,0,0 };
static Vector3 VECTOR3_FORWARD = { 0,0,1 };
static Vector3 VECTOR3_BACKWARDS = { 0,0,-1 };

static Vector3 VECTOR3_ZERO = { 0,0,0 };
static Vector3 VECTOR3_ONE = { 1,1,1 };
