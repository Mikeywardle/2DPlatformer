#pragma once

#include <Maths/Vector3.h>
#include <ecs/Entity.h>

struct Ray
{
	Ray(Vector3 Start, Vector3 End);

	Vector3 Start;
	Vector3 End;
	Vector3 Direction;
};

struct RaycastingResult
{
	bool hasHit;

	Entity HitEntity;
	float Distance;

	Vector3 Position;
	Vector3 Normal;
};