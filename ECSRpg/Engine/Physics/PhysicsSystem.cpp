#include "PhysicsSystem.h"

#include "RigidBody.h"
#include <Maths/Transform.h>
#include <Maths/Vector3.h>
#include "Collisions/CollisionSystem.h"
#include "Collisions/CollisionEvent.h"
#include <math.h>

#include "ForceHandler.h"
#include "Forces/Gravity.h"
#include "Forces/AirResistance.h"

#include "PhysicsWorld.h"
#include "PhysicsSystemConfig.h"


PhysicsSystem::PhysicsSystem(World* world, PhysicsSystemConfig physicsConfig)
{
	this->world = world;

	forceHandlers =
	{
		new GravityForceHandler(),
		new AirResistanceForceHandler()
	};

	this->collisionSystem = new CollisionSystem(world);
	physicsWorld = new PhysicsWorld(world, physicsConfig.maxCollisionLayers);
}

PhysicsSystem::~PhysicsSystem()
{
	delete(collisionSystem);
	delete(physicsWorld);
}

void PhysicsSystem::ProcessPhysicsForFrame(float deltaTime)
{
	ProcessForceHandlers(deltaTime);
	IntegrateForces(deltaTime);

	collisionSystem->ProcessCollisions(physicsWorld);
}

void PhysicsSystem::GenerateStaticWorld()
{
	collisionSystem->GenerateStaticColiisions(physicsWorld);
}

void PhysicsSystem::ClearWorld()
{
	physicsWorld->ClearDynamicWorld();
	physicsWorld->ClearStaticWorld();
}

void PhysicsSystem::ProcessForceHandlers(float deltaTime)
{
	for (IForceHandler* handler : forceHandlers)
	{
		handler->ApplyForceForFrame(world, deltaTime);
	}
}

void PhysicsSystem::IntegrateForces(float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<RigidBodyComponent, Transform>();

	const float MaxPhysicsFrameTime = .1f;

	deltaTime = fminf(deltaTime, MaxPhysicsFrameTime);

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		RigidBodyComponent* rigidBody = world->GetComponent<RigidBodyComponent>(entity);

		Vector3 frameResultant = rigidBody->GetResultantForce();

		rigidBody->velocity += frameResultant * rigidBody->GetInverseMass() * deltaTime;
		transform->AddTranslation(rigidBody->velocity * deltaTime);
		rigidBody->ClearResultantForce();
	}
}
