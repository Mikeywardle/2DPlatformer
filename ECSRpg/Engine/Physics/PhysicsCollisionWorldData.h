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
	PhysicsCollisionWorldData() = default;
	PhysicsCollisionWorldData(Entity Entity, Vector3 InPosition, Vector3 HalfLimits, ColliderType InType, bool isTrigger);

public:
	Entity entity : 27;
	Entity IsTrigger : 1;
	Entity type : 4;

	CollisionAABB BoundingBox;

public:

	CollisionAABB GetAABB() const;

	bool operator < (const PhysicsCollisionWorldData& in) const { return entity < in.entity; };
};
#pragma pack(pop)