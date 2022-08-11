#pragma once

#include <vector>
#include <ecs/Entity.h>
#include <Physics/Collisions/PhysicsCollisionResult.h>


struct PhysicsCollisionEnd
{
	PhysicsCollisionEnd(Entity entityA, Entity entityB);

	Entity entityA;
	Entity entityB;
};

struct CollisionEvent
{
	std::vector<PhysicsCollisionResult> results;
};

struct CollisionBeginEvent
{
	std::vector<PhysicsCollisionResult> results;
};

struct CollisionEndEvent
{
	std::vector<PhysicsCollisionEnd> results;
};

struct TriggerOverlapEvent
{
	std::vector<PhysicsCollisionResult> results;
};

struct TriggerOverlapBeginEvent
{
	std::vector<PhysicsCollisionResult> results;
};

struct TriggerOverlapEndEvent
{
	std::vector<PhysicsCollisionEnd> results;
};