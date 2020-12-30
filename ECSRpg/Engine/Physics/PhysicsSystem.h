#pragma once
#include <vector>
#include <Core/World.h>
#include "Collisions/CollisionSystem.h"

class RigidBodyComponent;

class PhysicsSystem
{
public:
	PhysicsSystem(World* world);
	void ProcessPhysicsForFrame(float deltaTime);

	template<typename ...Args>
	std::vector<Entity> CastSphere(struct Vector3 position, float radius)
	{
		return collisionSystem->CastSphere<Args...>(position, radius);
	}

	template<typename ...Args>
	std::vector<Entity> CastBox(struct Vector3 position, float halfWidth, float halfHeight, float halfDepth)
	{
		return collisionSystem->CastBox<Args...>(position, halfWidth, halfHeight, halfDepth);
	}

	template<typename ...Args>
	RaycastingResult CastRay(Vector3 Start, Vector3 End, Entity ignoreEntity)
	{
		return collisionSystem->CastRay<Args...>(Start, End, ignoreEntity);
	}

private:
	void ProcessForceHandlers(float deltaTime);
	void IntegrateForces(float deltaTime);

	void ResolveCollisions(struct CollisionEvent collisionResults);

	void AddImpulseIfNotNull(RigidBodyComponent* rba, const Vector3& frictionImpulse);

	//TODO Creat Collision Resolver class
	Vector3 GetRelativeVelocity(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetSeparatingVelocity(Vector3 RelativeVelocity, Vector3 Normal);
	float GetCollisionRestitution(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetTotalInversMass(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetTotalFrictionCoefficient(RigidBodyComponent* rb1, RigidBodyComponent* rb2);

	std::vector<class IForceHandler*> forceHandlers;

	World* world;

	CollisionSystem* collisionSystem;
};
