#pragma once

#include <Maths/MathsTypes.h>
#include <ecs/Entity.h>
#include <Physics/Collisions/CollisionComponents.h>
#include <Collisions/CollisionShapes.h>

struct CollisionAABB;

struct PhysicsCollisionWorldData
{
	PhysicsCollisionWorldData(Entity Entity, Vector3 InPosition, Vector3 HalfLimits, ColliderType InType);

	Entity entity;

	CollisionAABB BoundingBox;
	CollisionAABB2D BoundingBox2D;

	bool IsTrigger;
	ColliderType type;

	CollisionAABB GetAABB() const;
	CollisionAABB2D GetAABB2D() const;
};