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
