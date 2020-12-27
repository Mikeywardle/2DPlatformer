#include "SphereCollider.h"

SphereColliderComponent::SphereColliderComponent(float radius)
{
	this->radius = radius;
}

CollisionSphere::CollisionSphere(Entity entity, float Radius, Vector3 Position)
{
	this->entity = entity;
	this->Radius = Radius;
	this->Position = Position;
}

CollisionResult TestSpherevsSphere(CollisionSphere& A, CollisionSphere& B)
{
	CollisionResult result = CollisionResult();

	const Vector3 PositionsDiff = A.Position - B.Position;
	const float CollisionDepth = (A.Radius + B.Radius) - Vector3::Magnitude(PositionsDiff);

	result.entityA = A.entity;
	result.entityB = B.entity;

	result.hasCollision = CollisionDepth >= 0;
	result.collisionDepth = CollisionDepth;
	result.normal = Vector3::Normalize(PositionsDiff);

	return result;
}
