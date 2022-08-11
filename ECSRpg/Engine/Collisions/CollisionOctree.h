#pragma once

#include <Maths/Vector3.h>
#include <Collisions/CollisionShapes.h>
#include <Collisions/CollisionDetection.h>

#include "ICollisionCollection.h"

template<typename T>
struct CollisionOctreeNode
{
public:

	~CollisionQuadtreeNode<T>()
	{
		delete(Children[0]);
		delete(Children[1]);
		delete(Children[2]);
		delete(Children[3]);
		delete(Children[4]);
		delete(Children[5]);
		delete(Children[6]);
		delete(Children[7]);
	};

	std::vector<T> data;

	CollisionOctreeNode<T>* children [8] { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
};

template<typename T>
class CollisionOctree : public ICollisionCollection<T>
{
public:

	virtual void Insert(const T& item) override;

	virtual void Clear() override;
	virtual void SetDimensions(const Vector3 centre, const Vector3 halflimits, const Vector3 size) override;
	virtual void SetDataArrayLength(const int& length) override;
	virtual void Build() override;

	virtual void Query(const CollisionAABB& QueryAABB, std::vector<T>& results) override;
	virtual RaycastingResult CastRay(const Ray ray, const unsigned int IgnoreEntity, ICollisionCollection::RaycastingNarrowPhaseCallback callback, const void* userData) const override;

public:
	static const int MaxTreeDepth;

private:

	CollisionOctreeNode* root;

	Vector3 centre;
	Vector3 halfLimits;
};

template<typename T>
inline void CollisionOctree<T>::Insert(const T& item)
{
}
