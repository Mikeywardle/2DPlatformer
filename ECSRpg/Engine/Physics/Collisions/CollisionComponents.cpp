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

CollisionAABB2D AABBColliderComponent::GetAABB2DLimits(Transform* transform) const
{
	const Vector3 position = transform->GetPosition();
	const Vector2 position2D = Vector2(position.x, position.z);
	const Vector2 halfLimits2D = Vector2(HalfLimits.x, HalfLimits.z);

	return CollisionAABB2D(position2D, halfLimits2D);
}


SphereColliderComponent::SphereColliderComponent(float radius)
{
	this->radius = radius;
}

CollisionAABB SphereColliderComponent::GetAABBLimits(Transform* transform) const
{
	return CollisionAABB(transform->GetPosition(), Vector3(radius, radius, radius));
}

CollisionAABB2D SphereColliderComponent::GetAABB2DLimits(Transform* transform) const
{
	const Vector3 position = transform->GetPosition();
	const Vector2 position2D = Vector2(position.x, position.z);

	return CollisionAABB2D(position2D, Vector2(radius, radius));
}

CollisionSphere SphereColliderComponent::GetCollisionSphere(Transform* transform) const
{
	return CollisionSphere(radius, transform->GetPosition());
}
