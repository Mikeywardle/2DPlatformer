#include "CollisionDetection.h"

#include <math.h>

bool TestAABB2DvsAABB2DSimple(const CollisionAABB2D& A, const CollisionAABB2D& B)
{
	const Vector2 CollisionDepths = Vector2(
		(A.HalfLimits.x + B.HalfLimits.x) - fabsf(A.Position.x - B.Position.x),
		(A.HalfLimits.y + B.HalfLimits.y) - fabsf(A.Position.y - B.Position.y)
	);

	return CollisionDepths.x >= 0 && CollisionDepths.y >= 0;
}

bool TestAABB2DContainsAABB2D(const CollisionAABB2D& Container, const CollisionAABB2D& Containee)
{
	const Vector2 PositionDiff = Containee.Position - Container.Position;

	const float LongestXDistance = fmaxf(fabsf(PositionDiff.x + Containee.HalfLimits.x), fabsf(PositionDiff.x - Containee.HalfLimits.x));
	const float LongestYDistance = fmaxf(fabsf(PositionDiff.y + Containee.HalfLimits.y), fabsf(PositionDiff.y - Containee.HalfLimits.y));

	return LongestXDistance <= Container.HalfLimits.x
		&& LongestYDistance <= Container.HalfLimits.y;
}

CollisionResult TestABBvAABB(const CollisionAABB& A, const CollisionAABB& B)
{
	CollisionResult result;
	const Vector3 NormalVector = Vector3::Normalize(A.Position - B.Position);

	const Vector3 CollisionDepths = Vector3(
		(A.HalfLimits.x + B.HalfLimits.x) - fabsf(A.Position.x - B.Position.x),
		(A.HalfLimits.y + B.HalfLimits.y) - fabsf(A.Position.y - B.Position.y),
		(A.HalfLimits.z + B.HalfLimits.z) - fabsf(A.Position.z - B.Position.z)
	);

	result.hasCollision = CollisionDepths.x >= 0 && CollisionDepths.y >= 0 && CollisionDepths.z >= 0;

	float SmallestElement = CollisionDepths.x;
	Vector3 ResolutionDirection = Vector3::Right;

	if (CollisionDepths.y < SmallestElement)
	{
		SmallestElement = CollisionDepths.y;
		ResolutionDirection = Vector3::Up;
	}

	if (CollisionDepths.z < SmallestElement)
	{
		SmallestElement = CollisionDepths.z;
		ResolutionDirection = Vector3::Forwards;
	}

	if (Vector3::DotProduct(NormalVector, ResolutionDirection) < 0)
		ResolutionDirection *= -1;

	result.collisionDepth = SmallestElement;
	result.normal = ResolutionDirection;

	return result;

}

bool TestAABBvsAABBSimple(const CollisionAABB& A, const CollisionAABB& B)
{
	const Vector3 CollisionDepths = Vector3(
		(A.HalfLimits.x + B.HalfLimits.x) - fabsf(A.Position.x - B.Position.x),
		(A.HalfLimits.y + B.HalfLimits.y) - fabsf(A.Position.y - B.Position.y),
		(A.HalfLimits.z + B.HalfLimits.z) - fabsf(A.Position.z - B.Position.z)
	);

	return CollisionDepths.x >= 0 && CollisionDepths.y >= 0 && CollisionDepths.z >= 0;
}

CollisionResult TestSpherevsSphere(const CollisionSphere& A, const CollisionSphere& B)
{
	CollisionResult result = CollisionResult();

	const Vector3 PositionsDiff = A.Position - B.Position;
	const float CollisionDepth = (A.Radius + B.Radius) - Vector3::Magnitude(PositionsDiff);

	result.hasCollision = CollisionDepth >= 0;
	result.collisionDepth = CollisionDepth;
	result.normal = Vector3::Normalize(PositionsDiff);

	return result;
}

CollisionResult TestSphereVsAABB(const CollisionSphere& sphere, const CollisionAABB& box)
{
	CollisionResult result;
	const Vector3 NormalVector = Vector3::Normalize(sphere.Position - box.Position);

	const Vector3 CollisionDepths = Vector3(
		(sphere.Radius + box.HalfLimits.x) - fabsf(sphere.Position.x - box.Position.x),
		(sphere.Radius + box.HalfLimits.y) - fabsf(sphere.Position.y - box.Position.y),
		(sphere.Radius + box.HalfLimits.z) - fabsf(sphere.Position.z - box.Position.z)
	);

	result.hasCollision = CollisionDepths.x >= 0 && CollisionDepths.y >= 0 && CollisionDepths.z >= 0;

	if (!result.hasCollision)
		return result;

	//Find Better way to do this
	float SmallestElement = CollisionDepths.x;
	Vector3 ResolutionDirection = Vector3::Right;

	if (CollisionDepths.y < SmallestElement)
	{
		SmallestElement = CollisionDepths.y;
		ResolutionDirection = Vector3::Up;
	}

	if (CollisionDepths.z < SmallestElement)
	{
		SmallestElement = CollisionDepths.z;
		ResolutionDirection = Vector3::Forwards;
	}

	if (Vector3::DotProduct(NormalVector, ResolutionDirection) < 0)
		ResolutionDirection *= -1;

	result.collisionDepth = SmallestElement;
	result.normal = ResolutionDirection;

	return result;
}

//Rays
RaycastingResult TestRayVsSphere(const Ray& ray, const CollisionSphere& sphere)
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
	result.Distance = fmaxf(-fb - sqrtf(discriminant),0)/ray.Length;
	result.Position = ray.Start + (ray.Direction * result.Distance);
	result.Normal = Vector3::Normalize(result.Position - sphere.Position);

	return result;
}

RaycastingResult TestRayVsAABB(const Ray& ray, const CollisionAABB& box)
{
	RaycastingResult result;

	const Vector3 InverserRayDirection = Vector3(1/ray.Direction.x, 1 / ray.Direction.y, 1 / ray.Direction.z);

	const Vector3 lb = box.Position + box.HalfLimits;
	const Vector3 rt = box.Position - box.HalfLimits;

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
	result.Distance = tmin/ray.Length;
	result.Position = ray.Start + (ray.Direction * result.Distance);

	//result.Normal = Vector3::Normalize(result.Position - sphere.Position);

	return result;
}
