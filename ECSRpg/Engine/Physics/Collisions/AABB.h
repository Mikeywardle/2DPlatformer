#pragma once

#include <Maths/Vector3.h>
#include <ecs/Entity.h>
#include "CollisionResult.h"
struct AABBColliderComponent
{
	AABBColliderComponent() = default;
	AABBColliderComponent(float halfWidth, float halfHeight, float halfDepth);

	float halfWidth;
	float halfHeight;
	float halfDepth;
};

struct CollisionAABB
{
	CollisionAABB() = default;
	CollisionAABB(Entity entity, Vector3 Position, float halfWidth, float halfHeight, float halfDepth);

	Entity entity;
	Vector3 Position;
	float halfWidth;
	float halfHeight;
	float halfDepth;
};

CollisionResult TestABBvAABB(CollisionAABB& A, CollisionAABB& B);
