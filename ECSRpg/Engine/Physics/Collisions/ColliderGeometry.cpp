#include "ColliderGeometry.h"

AABBCollisionGeometry::AABBCollisionGeometry(Vector3 halfLimits)
{
	this->HalfLimits = halfLimits;
}

CollisionAABB AABBCollisionGeometry::GetAABBLimits(const SceneTransformComponent* transform) const
{
	return CollisionAABB(transform->GetPosition(), HalfLimits);
}

SphereCollisionGeometry::SphereCollisionGeometry(float radius)
{
	this->radius = radius;
}

CollisionAABB SphereCollisionGeometry::GetAABBLimits(const SceneTransformComponent* transform) const
{
	return CollisionAABB(transform->GetPosition(), Vector3(radius, radius, radius));
}

CollisionSphere SphereCollisionGeometry::GetCollisionSphere(const SceneTransformComponent* transform) const
{
	return CollisionSphere(radius, transform->GetPosition());
}