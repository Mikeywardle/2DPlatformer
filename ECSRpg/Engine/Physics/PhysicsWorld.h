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

typedef unsigned int Entity;


typedef PhysicsCollisionResult(*FunctionCallBack)(IColliderComponent* colliderA, Transform* transformA, World* world);

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

	int GetColliderPairingIndex(const int8& a, const int8& b);
	int GetTotalNumberOfColliderPairings();

private:

	World* world;

	std::vector<FunctionCallBack> CollisionFunctionsCallbacks;

	CollisionQuadtree<PhysicsCollisionWorldData>* dynamicTree;
	CollisionQuadtree<PhysicsCollisionWorldData>* staticTree;
};