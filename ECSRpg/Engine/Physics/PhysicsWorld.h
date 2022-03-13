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

struct RaycastingResult;
struct Ray;

typedef unsigned int Entity;

typedef CollisionResult (*CollisionFunctionCallBack)(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world);

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
		, const IColliderComponent* collider
		, Transform* transform
		, const std::vector<uint8>& collisionLayers
	) const;

	Vector3 GetGravity() const;
	int GetTotalCollisionLayers() const {return totalCollisionLayers;};

	RaycastingResult CastRay(const Ray ray, const std::vector<uint8> collisionLayers, const Entity toIgnoreEntity) const;

	void BoxCast() const;
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
	CollisionResult AABBvsAABB(const IColliderComponent * colliderA, Transform * transformA, const PhysicsCollisionWorldData entityB, World * world);
	CollisionResult AABBvsSphere(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world);

	//Sphere vs
	CollisionResult SpherevsAABB(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world);
	CollisionResult SpherevsSphere(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world);

	//Raycast Callback
	RaycastingResult RaycastingCallback(const Ray ray, const PhysicsCollisionWorldData item, const unsigned int IgnoreEntity, const void* userData);
}
