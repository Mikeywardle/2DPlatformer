#include "CollisionDetection.h"

#include <math.h>

CollisionResult TestSphereVsAABB(CollisionSphere& sphere, CollisionAABB& box)
{
	CollisionResult result;
	const Vector3 NormalVector = Vector3::Normalize(sphere.Position - box.Position);

	const Vector3 CollisionDepths = Vector3(
		(sphere.Radius + box.halfWidth) - fabsf(sphere.Position.x - box.Position.x),
		(sphere.Radius + box.halfHeight) - fabsf(sphere.Position.y - box.Position.y),
		(sphere.Radius + box.halfDepth) - fabsf(sphere.Position.z - box.Position.z)
	);

	result.entityA = sphere.entity;
	result.entityB = box.entity;

	result.hasCollision = CollisionDepths.x >= 0 && CollisionDepths.y >= 0 && CollisionDepths.z >= 0;

	if (!result.hasCollision)
		return result;

	const float halfSphereRadius = sphere.Radius / 2;
	const bool onXEdge = CollisionDepths.x < halfSphereRadius;
	const bool onYEdge = CollisionDepths.y < halfSphereRadius;
	const bool onZEdge = CollisionDepths.z < halfSphereRadius;

	const bool isOnEdge = onXEdge + onYEdge + onZEdge >= 2;

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

//Rays
RaycastingResult TestRayVsSphere(Ray& ray, CollisionSphere& sphere)
{
	RaycastingResult result;

	const Vector3 diff = ray.Start - sphere.Position;
	const float fb = Vector3::DotProduct(diff, ray.Direction);
	const float fc = Vector3::DotProduct(diff, diff) - (sphere.Radius * sphere.Radius);

	if (fc > 0.0f && fb > 0.0f)
	{
		result.hasHit = false;
		return result;
	}

	const float discriminant = (fb * fb) - fc;

	if (discriminant < 0)
	{
		result.hasHit = false;
		return result;
	}

	//Ray has hit

	result.hasHit = true;
	result.HitEntity = sphere.entity;
	result.Distance = fmaxf(-fb - sqrtf(discriminant),0);
	result.Position = ray.Start + (ray.Direction * result.Distance);
	result.Normal = Vector3::Normalize(result.Position - sphere.Position);

	return result;
}

RaycastingResult TestRayVsAABB(Ray& ray, CollisionAABB& sphere)
{
	return RaycastingResult();
}
