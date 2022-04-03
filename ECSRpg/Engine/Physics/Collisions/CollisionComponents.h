#pragma once
#include <Maths/MathsTypes.h>
#include <Core/SceneTransformComponents.h>

#include <memory>

#include <Collisions/CollisionShapes.h>

#include "ColliderGeometry.h"

#include <vector>

struct DynamicCollider {};
struct StaticCollider {};

struct ColliderGeometryComponent
{
public:

	bool isTrigger = false;
	uint8 collisionLayer = 0;

	std::vector<uint8> toCollideLayers;

	ICollisionGeometry* GetCollisionGeometry() const;
	void SetCollisionGeometry(ICollisionGeometry* inGeometry);

	ColliderType GetColliderType() const;

	CollisionAABB GetAABBLimits(const SceneTransformComponent* transform) const;

	AABBCollisionGeometry* GetAABBGeometry() const;
	SphereCollisionGeometry* GetSphereGeometry() const;

private:
	std::shared_ptr<ICollisionGeometry> geometry{ nullptr };
};


