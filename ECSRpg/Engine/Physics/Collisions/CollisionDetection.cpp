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

	//const bool onXEdge = CollisionDepths.x < sphere.Radius;
	//const bool onYEdge = CollisionDepths.y < sphere.Radius;
	//const bool onZEdge = CollisionDepths.z < sphere.Radius;

	//const bool isOnEdge = onXEdge + onYEdge + onZEdge >= 2;


	//Find Better way to do this
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

RaycastingResult TestRayVsAABB(Ray& ray, CollisionAABB& box)
{
	RaycastingResult result;

	const Vector3 InverserRayDirection = Vector3(1/ray.Direction.x, 1 / ray.Direction.y, 1 / ray.Direction.z);

	const Vector3 lb = box.Position + Vector3(box.halfWidth, box.halfHeight, box.halfDepth);
	const Vector3 rt = box.Position - Vector3(box.halfWidth, box.halfHeight, box.halfDepth);

	float t1 = (lb.x - ray.Start.x) * InverserRayDirection.x;
	float t2 = (rt.x - ray.Start.x) * InverserRayDirection.x;
	float t3 = (lb.y - ray.Start.y) * InverserRayDirection.y;
	float t4 = (rt.y - ray.Start.y) * InverserRayDirection.y;
	float t5 = (lb.z - ray.Start.z) * InverserRayDirection.z;
	float t6 = (rt.z - ray.Start.z) * InverserRayDirection.z;

	float tmin = fmaxf(fmaxf(fminf(t1, t2),fminf(t3, t4)), fminf(t5, t6));
	float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0
		|| tmin > tmax
		|| tmin > ray.Length)
	{
		result.hasHit = false;
		return result;
	}

	result.hasHit = true;
	result.HitEntity = box.entity;
	result.Distance = tmin/ray.Length;
	result.Position = ray.Start + (ray.Direction * result.Distance);

	//result.Normal = Vector3::Normalize(result.Position - sphere.Position);

	return result;
}
