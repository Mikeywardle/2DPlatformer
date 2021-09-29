#pragma once
#include <Maths/MathsTypes.h>
#include <Maths/Transform.h>
#include <Collisions/CollisionShapes.h>

struct DynamicCollider {};
struct StaticCollider {};

//TODO: Add OBB, Capsule, Cylinder, Box soup?
enum class ColliderType : int8
{
	None = -1
	, AABB = 0
	, Sphere = 1
	, TotalCollisionComponents = 2
};

struct ColliderMetaComponent
{
	ColliderType type = ColliderType::None;
	bool isTrigger = false;
};

struct IColliderComponent 
{
	virtual CollisionAABB GetAABBLimits(Transform* transform) const  = 0;
	virtual CollisionAABB2D GetAABB2DLimits(Transform* transform) const  = 0;
	virtual ColliderType GetColliderType() const = 0;
};

struct AABBColliderComponent final : public IColliderComponent
{
public:
	AABBColliderComponent() = default;
	AABBColliderComponent(Vector3 halfLimits);

	virtual CollisionAABB GetAABBLimits(Transform* transform) const override;
	virtual CollisionAABB2D GetAABB2DLimits(Transform* transform) const override;
	virtual ColliderType GetColliderType() const override { return ColliderType::AABB; };

public:
	Vector3 HalfLimits;
};

struct SphereColliderComponent final : public IColliderComponent
{
public:
	SphereColliderComponent() = default;
	SphereColliderComponent(float radius);

	virtual CollisionAABB GetAABBLimits(Transform* transform) const override;
	virtual CollisionAABB2D GetAABB2DLimits(Transform* transform) const override;
	virtual ColliderType GetColliderType() const override { return ColliderType::Sphere; };

public:

	float radius;
};
