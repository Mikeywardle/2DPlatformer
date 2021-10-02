#pragma once

#include <vector>
#include<Maths/MathsTypes.h>

class World;

template <typename T>
class CollisionQuadtree;

struct PhysicsCollisionWorldData;
struct PhysicsCollisionResult;
struct Transform;
struct ColliderMetaComponent;
struct IColliderComponent;
struct Vector2;
struct Vector3;
struct CollisionResult;

typedef unsigned int Entity;


typedef CollisionResult (*FunctionCallBack)(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world);

class PhysicsWorld
{
public:

	PhysicsWorld(World* InWorld);
	~PhysicsWorld();

	void ClearDynamicWorld();
	void ClearStaticWorld();

	void AddDynamicBody(const PhysicsCollisionWorldData& toAdd);
	void AddStaticBody(const PhysicsCollisionWorldData& toAdd);

	void SetDynamicWorldLimits(const Vector2& position, const Vector2& halfLimits);
	void SetStaticWorldLimits(const Vector2& position, const Vector2& halfLimits);

	void QueryCollider(std::vector<PhysicsCollisionResult>& results, const PhysicsCollisionWorldData query, const IColliderComponent* collider, Transform* transform) const;

	Vector3 GetGravity() const;

	void BoxCast() const;
	void SphereCast() const;
	void CastRay() const;



private:

	int GetColliderPairingIndex(const int8& a, const int8& b) const;
	int GetTotalNumberOfColliderPairings();

private:

	World* world;

	std::vector<FunctionCallBack> CollisionFunctionsCallbacks;

	CollisionQuadtree<PhysicsCollisionWorldData>* dynamicTree;
	CollisionQuadtree<PhysicsCollisionWorldData>* staticTree;

	const float QuadTreeBorderWidth = 10.f;
};

namespace PhysicsWorldCollisionFunctions
{
	//AABB vs
	CollisionResult AABBvsAABB(const IColliderComponent * colliderA, Transform * transformA, const PhysicsCollisionWorldData entityB, World * world);
	CollisionResult AABBvsSphere(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world);

	//Sphere vs
	CollisionResult SpherevsAABB(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world);
	CollisionResult SpherevsSphere(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world);
}