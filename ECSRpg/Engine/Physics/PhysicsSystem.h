#pragma once
#include <vector>
#include <Core/World.h>
#include "Collisions/CollisionSystem.h"

class RigidBodyComponent;
struct PhysicsSystemConfig;
struct PhysicsCollisionCastResult;

class PhysicsSystem
{
public:
	PhysicsSystem(World* world, PhysicsSystemConfig physicsConfig);
	~PhysicsSystem();

	void ProcessPhysicsForFrame(float deltaTime);


	void GenerateStaticWorld();
	void ClearWorld();

	RaycastingResult CastRay(const Ray ray, const Entity ignoreEntity = NO_ENTITY, const std::vector<uint8> layers = std::vector<uint8>()) const;
	std::vector<PhysicsCollisionCastResult> CastBox(const CollisionAABB box, const std::vector<uint8> layers = std::vector<uint8>()) const;

private:
	void ProcessForceHandlers(float deltaTime);
	void IntegrateForces(float deltaTime);

	std::vector<class IForceHandler*> forceHandlers;

	World* world;

	CollisionSystem* collisionSystem;
	PhysicsWorld* physicsWorld;
};
