#pragma once

#include <vector>

struct Vector2;
struct PhysicsCollisionWorldData;
struct PhysicsCollisionResult;

template <typename T>
class CollisionQuadtree;

class PhysicsCollisionLayer
{
public:
	PhysicsCollisionLayer();
	~PhysicsCollisionLayer();

	void ClearDynamicObjects();
	void ClearStaticObjects();

	void AddDynamicBody(const PhysicsCollisionWorldData& toAdd);
	void AddStaticBody(const PhysicsCollisionWorldData& toAdd);

	void SetDynamicLimits(const Vector2& position, const Vector2& halfLimits);
	void SetStaticLimits(const Vector2& position, const Vector2& halfLimits);

	void QueryCollider(std::vector<PhysicsCollisionWorldData>& colliderOverlapResults, std::vector<PhysicsCollisionWorldData>& triggerOverlapResults, const PhysicsCollisionWorldData query) const;

private:

	CollisionQuadtree<PhysicsCollisionWorldData>* dynamicColliders;
	CollisionQuadtree<PhysicsCollisionWorldData>* dynamicTriggers;

	CollisionQuadtree<PhysicsCollisionWorldData>* staticColliders;
	CollisionQuadtree<PhysicsCollisionWorldData>* staticTriggers;

	static const float QuadTreeBorderWidth;
};