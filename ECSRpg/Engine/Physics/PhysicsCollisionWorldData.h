#pragma once

#include <Maths/MathsTypes.h>
#include <ecs/Entity.h>
#include <Physics/Collisions/CollisionComponents.h>
#include <Collisions/CollisionShapes.h>

struct CollisionAABB;

#pragma pack(push, 1)
struct PhysicsCollisionWorldData
{
public:
	PhysicsCollisionWorldData(Entity Entity, Vector3 InPosition, Vector3 HalfLimits, ColliderType InType, bool isTrigger);

public:
	Entity entity;

	CollisionAABB BoundingBox;

	bool IsTrigger = false;
	ColliderType type;

public:

	CollisionAABB GetAABB() const;

	bool operator < (const PhysicsCollisionWorldData& in) const { return entity < in.entity; };
};

#pragma pack(pop)