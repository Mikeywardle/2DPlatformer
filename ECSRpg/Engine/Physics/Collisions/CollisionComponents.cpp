#include "CollisionComponents.h"
#include <Maths/Transform.h>


ICollisionGeometry* ColliderGeometryComponent::GetCollisionGeometry() const
{
	return geometry.get();
}

void ColliderGeometryComponent::SetCollisionGeometry(ICollisionGeometry* inGeometry)
{
	geometry = std::shared_ptr<ICollisionGeometry>(inGeometry);
}

ColliderType ColliderGeometryComponent::GetColliderType() const
{
	return geometry->GetColliderType();
}

CollisionAABB ColliderGeometryComponent::GetAABBLimits(const Transform* transform) const
{
	return geometry->GetAABBLimits(transform);
}

AABBCollisionGeometry* ColliderGeometryComponent::GetAABBGeometry() const
{
	return dynamic_cast<AABBCollisionGeometry*>(geometry.get());
}

SphereCollisionGeometry* ColliderGeometryComponent::GetSphereGeometry() const
{
	return dynamic_cast<SphereCollisionGeometry*>(geometry.get());
}
