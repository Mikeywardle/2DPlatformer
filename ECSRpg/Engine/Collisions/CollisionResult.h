#pragma once

#include <Maths/Vector3.h>

struct CollisionResult
{
	bool hasCollision;

	float collisionDepth;
	Vector3 normal;
};

CollisionResult FlipCollisionResult(CollisionResult result);