#pragma once

#include <ecs/Entity.h>
#include <Maths/Vector3.h>

struct CollisionResult
{
	bool hasCollision;

	Entity entityA;
	Entity entityB;

	float collisionDepth;
	Vector3 normal;

};

void FlipCollisionResult(CollisionResult& result);
