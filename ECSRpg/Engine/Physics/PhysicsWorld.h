#pragma once

#include <vector>
#include<Maths/MathsTypes.h>

class World;

struct PhysicsCollisionWorldData;
struct PhysicsCollisionResult;
struct PhysicsCollisionLayer;

struct ColliderGeometryComponent;
struct CollisionResult;
struct CollisionAABB;
struct PhysicsCollisionCastResult;

struct Transform;
struct Vector2;
struct Vector3;

struct RaycastingResult;
struct Ray;

typedef unsigned int Entity;

typedef CollisionResult (*CollisionFunctionCallBack)(const ColliderGeometryComponent* colliderA, const Transform* transformA, const ColliderGeometryComponent* colliderB, const Transform* transformB);

class PhysicsWorld
{
public:

	PhysicsWorld(World* InWorld, const uint8 TotalCollisionLayers);
	~PhysicsWorld();

	void ClearDynamicWorld();
	void ClearStaticWorld();

	void AddDynamicBody(const PhysicsCollisionWorldData& toAdd, const uint8 CollisionLayer);
	void AddStaticBody(const PhysicsCollisionWorldData& toAdd, const uint8 CollisionLayer);

	void SetDynamicWorldLimits(const Vector3& position, const Vector3& limits);
	void SetStaticWorldLimits(const Vector3& position, const Vector3& limits);

	void QueryCollider(
		std::vector<PhysicsCollisionResult>& colliderResults
		, std::vector<PhysicsCollisionResult>& triggerResults
		, const PhysicsCollisionWorldData query
		, const ColliderGeometryComponent* collider
		, const Transform* transform
		, const std::vector<uint8>& collisionLayers
	) const;

	Vector3 GetGravity() const;
	int GetTotalCollisionLayers() const {return totalCollisionLayers;};

	RaycastingResult CastRay(const Ray ray, const std::vector<uint8> collisionLayers, const Entity toIgnoreEntity) const;
	std::vector<PhysicsCollisionCastResult> BoxCast(const CollisionAABB box, std::vector<uint8> collisionLayers) const;
	void SphereCast() const;

	PhysicsCollisionLayer* GetCollisionLayer(int index) const;

private:

	int GetColliderPairingIndex(const int8& a, const int8& b) const;
	int GetTotalNumberOfColliderPairings();

private:

	World* world;

	std::vector<CollisionFunctionCallBack> CollisionFunctionsCallbacks;
	std::vector<PhysicsCollisionLayer*> CollisionLayers;

	int totalCollisionLayers = 0;
};

namespace PhysicsWorldCollisionFunctions
{
	//AABB vs
	CollisionResult AABBvsAABB(const ColliderGeometryComponent* colliderA, const Transform* transformA, const ColliderGeometryComponent* colliderB, const Transform* transformB);
	CollisionResult AABBvsSphere(const ColliderGeometryComponent* colliderA, const Transform* transformA, const ColliderGeometryComponent* colliderB, const Transform* transformB);

	//Sphere vs
	CollisionResult SpherevsAABB(const ColliderGeometryComponent* colliderA, const Transform* transformA, const ColliderGeometryComponent* colliderB, const Transform* transformB);
	CollisionResult SpherevsSphere(const ColliderGeometryComponent* colliderA, const Transform* transformA, const ColliderGeometryComponent* colliderB, const Transform* transformB);

	//Raycast Callback
	RaycastingResult RaycastingCallback(const Ray ray, const PhysicsCollisionWorldData item, const unsigned int IgnoreEntity, const void* userData);
}
