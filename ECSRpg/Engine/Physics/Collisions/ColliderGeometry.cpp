#include "ColliderGeometry.h"

AABBCollisionGeometry::AABBCollisionGeometry(Vector3 halfLimits)
{
	this->HalfLimits = halfLimits;
}

CollisionAABB AABBCollisionGeometry::GetAABBLimits(const Transform* transform) const
{
	return CollisionAABB(transform->GetPosition(), HalfLimits * transform->GetScale());
}

SphereCollisionGeometry::SphereCollisionGeometry(float radius)
{
	this->radius = radius;
}

CollisionAABB SphereCollisionGeometry::GetAABBLimits(const Transform* transform) const
{
	return CollisionAABB(transform->GetPosition(), Vector3(radius, radius, radius));
}

CollisionSphere SphereCollisionGeometry::GetCollisionSphere(const Transform* transform) const
{
	return CollisionSphere(radius, transform->GetPosition());
}