#include "CollisionComponents.h"
#include <Maths/Transform.h>
#include <Maths/Vector2.h>

AABBColliderComponent::AABBColliderComponent(Vector3 halfLimits)
{
	this->HalfLimits = halfLimits;
}

CollisionAABB AABBColliderComponent::GetAABBLimits(Transform* transform) const
{
	return CollisionAABB(transform->GetPosition(), HalfLimits);
}

SphereColliderComponent::SphereColliderComponent(float radius)
{
	this->radius = radius;
}

CollisionAABB SphereColliderComponent::GetAABBLimits(Transform* transform) const
{
	return CollisionAABB(transform->GetPosition(), Vector3(radius, radius, radius));
}

CollisionSphere SphereColliderComponent::GetCollisionSphere(Transform* transform) const
{
	return CollisionSphere(radius, transform->GetPosition());
}
