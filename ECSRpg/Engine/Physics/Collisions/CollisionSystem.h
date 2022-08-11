#pragma once

#include <Core/World.h>
#include <Collisions/CollisionDetection.h>
#include <Physics/Collisions/CollisionComponents.h>

class World;

class PhysicsWorld;
struct PhysicsCollisionResult;

struct RigidBodyComponent;
struct Vector3;
struct PhysicsCollisionEnd;

typedef unsigned int Entity;

class CollisionSystem
{
public:
	CollisionSystem(World* world);

	void ProcessCollisions(PhysicsWorld* physWorld);

	void SetCollidersDirty();

	void GenerateStaticColiisions(PhysicsWorld* physWorld);

private:

	void FindDynamicCollisions(std::vector<PhysicsCollisionResult>& colliderResults, std::vector<PhysicsCollisionResult>& triggerResults, PhysicsWorld* physWorld);

	void ResolveCollisions(std::vector<PhysicsCollisionResult>& results);

	void AddImpulseIfNotNull(RigidBodyComponent* rba, const Vector3& frictionImpulse);
	Vector3 GetRelativeVelocity(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetSeparatingVelocity(Vector3 RelativeVelocity, Vector3 Normal);
	float GetCollisionRestitution(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetTotalInversMass(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetTotalFrictionCoefficient(RigidBodyComponent* rb1, RigidBodyComponent* rb2);

	void ProcessBeginCollisionEvents(std::vector<PhysicsCollisionResult>& results, std::vector<PhysicsCollisionResult>& begin	);
	void ProcessEndCollisionEvents(std::vector<PhysicsCollisionEnd>& colliders, std::vector<PhysicsCollisionEnd>& triggers);

private:

	World* world;

	struct CollisionWorldBoundariesData
	{
		bool first = true;
		Vector3 bottomLeft;
		Vector3 topRight;
	};
};
