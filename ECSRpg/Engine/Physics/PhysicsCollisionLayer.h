#pragma once

#include <vector>

struct Vector3;
struct PhysicsCollisionWorldData;
struct PhysicsCollisionResult;

struct RaycastingResult;
struct Ray;

template <typename T>
class CollisionGrid;

class World;

typedef RaycastingResult(*PhysicsRaycastingCallback)(const Ray ray, const PhysicsCollisionWorldData item, const unsigned int IgnoreEntity, const void* userData);

class PhysicsCollisionLayer
{
public:
	PhysicsCollisionLayer();
	~PhysicsCollisionLayer();

	void ClearDynamicObjects();
	void ClearStaticObjects();

	void AddDynamicBody(const PhysicsCollisionWorldData& toAdd);
	void AddStaticBody(const PhysicsCollisionWorldData& toAdd);

	void SetDynamicLimits(const Vector3& position, const Vector3& limits);
	void SetStaticLimits(const Vector3& position, const Vector3& limits);

	void SetLimits(const int& layerType, const Vector3& position, const Vector3& limits);
	void SetDataArrayLength(const int& layerType, const int& length);
	void BuildGridCells(const int& layerType);


	void QueryCollider(std::vector<PhysicsCollisionWorldData>& colliderOverlapResults, std::vector<PhysicsCollisionWorldData>& triggerOverlapResults, const PhysicsCollisionWorldData query) const;
	RaycastingResult CastRay(const Ray ray, const unsigned int IgnoreEntity, PhysicsRaycastingCallback callback, const World* world) const;

private:

	CollisionGrid<PhysicsCollisionWorldData>* dynamicColliders;
	CollisionGrid<PhysicsCollisionWorldData>* dynamicTriggers;

	CollisionGrid<PhysicsCollisionWorldData>* staticColliders;
	CollisionGrid<PhysicsCollisionWorldData>* staticTriggers;

	static const float BorderWidth;
};