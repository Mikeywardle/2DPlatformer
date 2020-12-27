#include "AABB.h"
#include <math.h>

AABBColliderComponent::AABBColliderComponent(float halfWidth, float halfHeight, float halfDepth)
{
	this->halfWidth = halfWidth;
	this->halfHeight = halfHeight;
	this->halfDepth = halfDepth;
}

CollisionResult TestABBvAABB(CollisionAABB& A, CollisionAABB& B) 
{
	CollisionResult result;
	Vector3 NormalVector = Vector3::Normalize(A.Position - B.Position);

	Vector3 CollisionDepths = Vector3(
		(A.halfWidth + B.halfWidth) - fabsf(A.Position.x - B.Position.x),
		(A.halfHeight + B.halfHeight) - fabsf(A.Position.y - B.Position.y),
		(A.halfDepth + B.halfDepth) - fabsf(A.Position.z - B.Position.z)
	);

	result.entityA = A.entity;
	result.entityB = B.entity;

	result.hasCollision = CollisionDepths.x >= 0 && CollisionDepths.y>=0 && CollisionDepths.z>=0;

	float SmallestElement = CollisionDepths.x;
	Vector3 ResolutionDirection = VECTOR3_RIGHT;

	if (CollisionDepths.y < SmallestElement)
	{
		SmallestElement = CollisionDepths.y;
		ResolutionDirection = VECTOR3_UP;
	}

	if (CollisionDepths.z < SmallestElement)
	{
		SmallestElement = CollisionDepths.z;
		ResolutionDirection = VECTOR3_FORWARD;
	}

	if (Vector3::DotProduct(NormalVector, ResolutionDirection) < 0)
		ResolutionDirection *= -1;

	result.collisionDepth = SmallestElement;
	result.normal = ResolutionDirection;

	return result;

}

CollisionAABB::CollisionAABB(Entity entity, Vector3 Position, float halfWidth, float halfHeight, float halfDepth)
{
	this->entity = entity;
	this->Position = Position;
	this->halfWidth = halfWidth;
	this->halfHeight = halfHeight;
	this->halfDepth = halfDepth;
}
