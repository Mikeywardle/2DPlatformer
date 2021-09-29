#pragma once

#include <ecs/Entity.h>
#include <Collisions/CollisionResult.h>

struct PhysicsCollisionResult
{
	Entity entityA;
	Entity entityB;

	bool IsTriggerOverlap;

	CollisionResult collisionInfo;
};
