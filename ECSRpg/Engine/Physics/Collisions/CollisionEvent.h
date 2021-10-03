#pragma once

#include <vector>
#include <Physics/Collisions/PhysicsCollisionResult.h>

struct CollisionEvent
{
	std::vector<PhysicsCollisionResult> results;
};

struct TriggerOverlapEvent
{
	std::vector<PhysicsCollisionResult> results;
};
