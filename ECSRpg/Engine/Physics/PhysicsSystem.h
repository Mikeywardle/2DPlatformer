#pragma once
#include <vector>
#include <Core/World.h>
#include "Collisions/CollisionSystem.h"

class RigidBodyComponent;

class PhysicsSystem
{
public:
	PhysicsSystem(World* world);
	~PhysicsSystem();

	void ProcessPhysicsForFrame(float deltaTime);


	void GenerateStaticWorld();

	template<typename ...Args>
	RaycastingResult CastRay(Vector3 Start, Vector3 End, Entity ignoreEntity)
	{
		return collisionSystem->CastRay<Args...>(Start, End, ignoreEntity);
	}

private:
	void ProcessForceHandlers(float deltaTime);
	void IntegrateForces(float deltaTime);

	std::vector<class IForceHandler*> forceHandlers;

	World* world;

	CollisionSystem* collisionSystem;
	PhysicsWorld* physicsWorld;
};
