#pragma once

#include <Maths/Vector3.h>
#include <Maths/Vector2.h>

struct CollisionAABB
{
	CollisionAABB() = default;
	CollisionAABB(Vector3 Position, Vector3 HalfLimits);

	Vector3 Position;
	Vector3 HalfLimits;
};

struct CollisionAABB2D
{
	CollisionAABB2D() = default;
	CollisionAABB2D(Vector2 Position, Vector2 HalfLimits);

	Vector2 Position;
	Vector2 HalfLimits;
};

struct CollisionSphere
{
	CollisionSphere() = default;
	CollisionSphere(float Radius, Vector3 Position);

	float Radius;
	Vector3 Position;
};

struct CollisionTriangle
{
	CollisionTriangle() = default;
	CollisionTriangle(Vector3 point0, Vector3 point1, Vector3 point2, Vector3 Normal );

	Vector3 point0;
	Vector3 point1;
	Vector3 point2;
	Vector3 Normal;
};

struct CollisionCapsule
{

};
