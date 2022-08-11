#pragma once

#include <vector>

struct Vector3;
struct Ray;
struct RaycastingResult;



template<typename T>
class ICollisionCollection
{
public:
	typedef RaycastingResult(*RaycastingNarrowPhaseCallback)(const Ray ray, const T item, const unsigned int IgnoreEntity, const void* userData);

	virtual void Insert(const T& item) = 0;

	virtual void Clear() = 0;
	virtual void SetDimensions(const Vector3 centre, const Vector3 halflimits, const Vector3 size) = 0;
	virtual void SetDataArrayLength(const int& length) = 0;
	virtual void Build() = 0;

	virtual void Query(const CollisionAABB& QueryAABB, std::vector<T>& results) const = 0;
	virtual RaycastingResult CastRay(const Ray ray, const unsigned int IgnoreEntity, RaycastingNarrowPhaseCallback callback, const void* userData) const = 0;
};
