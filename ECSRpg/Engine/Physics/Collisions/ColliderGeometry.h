#pragma once

#include <Maths/MathsTypes.h>
#include <Maths/Transform.h>

#include <Collisions/CollisionShapes.h>

//TODO: Add OBB, Capsule, Cylinder, Box soup?
enum class ColliderType : int8
{
	None = -1
	, AABB = 0
	, Sphere = 1
	, TotalCollisionComponents = 2
};

class ICollisionGeometry
{
public:

	virtual CollisionAABB GetAABBLimits(const Transform* transform) const = 0;
	virtual ColliderType GetColliderType() const = 0;
};


class AABBCollisionGeometry final : public ICollisionGeometry
{
public:
	AABBCollisionGeometry() = default;
	AABBCollisionGeometry(Vector3 halfLimits);

	virtual CollisionAABB GetAABBLimits(const Transform * transform) const override;
	virtual ColliderType GetColliderType() const override { return ColliderType::AABB; };

public:
	Vector3 HalfLimits;
};

class SphereCollisionGeometry final : public ICollisionGeometry
{
public:
	SphereCollisionGeometry() = default;
	SphereCollisionGeometry(float radius);

	virtual CollisionAABB GetAABBLimits(const Transform * transform) const override;
	virtual ColliderType GetColliderType() const override { return ColliderType::Sphere; };

	CollisionSphere GetCollisionSphere(const Transform * transform) const;

public:

	float radius;
};

