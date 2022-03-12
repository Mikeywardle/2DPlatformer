#pragma once

#include <Core/World.h>
#include <Collisions/CollisionDetection.h>
#include <Physics/Collisions/CollisionComponents.h>

class World;

class PhysicsWorld;
struct PhysicsCollisionResult;

struct ColliderMetaComponent;
struct IColliderComponent;
struct RigidBodyComponent;
struct Vector3;

typedef unsigned int Entity;

class CollisionSystem
{
public:
	CollisionSystem(World* world);

	void ProcessCollisions(PhysicsWorld* physWorld);

	void GenerateStaticColiisions(PhysicsWorld* physWorld);

private:

	void FindDynamicCollisions(std::vector<PhysicsCollisionResult>& colliderResults, std::vector<PhysicsCollisionResult>& triggerResults, PhysicsWorld* physWorld);
	IColliderComponent* GetEntityCollider(const Entity entity, const ColliderMetaComponent* metaData);

	void ResolveCollisions(std::vector<PhysicsCollisionResult>& results);

	void AddImpulseIfNotNull(RigidBodyComponent* rba, const Vector3& frictionImpulse);
	Vector3 GetRelativeVelocity(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetSeparatingVelocity(Vector3 RelativeVelocity, Vector3 Normal);
	float GetCollisionRestitution(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetTotalInversMass(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetTotalFrictionCoefficient(RigidBodyComponent* rb1, RigidBodyComponent* rb2);

	World* world;
};
