#pragma once

#include <vector>
#include<Maths/MathsTypes.h>

class World;


struct PhysicsCollisionWorldData;
struct PhysicsCollisionResult;
struct PhysicsCollisionLayer;

struct ColliderMetaComponent;
struct IColliderComponent;
struct CollisionResult;

struct Transform;
struct Vector2;
struct Vector3;

typedef unsigned int Entity;

typedef CollisionResult (*FunctionCallBack)(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world);

class PhysicsWorld
{
public:

	PhysicsWorld(World* InWorld, const uint8 TotalCollisionLayers);
	~PhysicsWorld();

	void ClearDynamicWorld();
	void ClearStaticWorld();

	void AddDynamicBody(const PhysicsCollisionWorldData& toAdd, const uint8 CollisionLayer);
	void AddStaticBody(const PhysicsCollisionWorldData& toAdd, const uint8 CollisionLayer);

	void SetDynamicWorldLimits(const Vector2& position, const Vector2& halfLimits);
	void SetStaticWorldLimits(const Vector2& position, const Vector2& halfLimits);

	void QueryCollider(
		std::vector<PhysicsCollisionResult>& colliderResults
		, std::vector<PhysicsCollisionResult>& triggerResults
		, const PhysicsCollisionWorldData query
		, const IColliderComponent* collider
		, Transform* transform
		, const std::vector<uint8>& collisionLayers
	) const;

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
	std::vector<PhysicsCollisionLayer*> CollisionLayers;

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