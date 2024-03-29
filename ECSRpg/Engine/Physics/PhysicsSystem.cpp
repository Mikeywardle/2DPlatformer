#include "PhysicsSystem.h"

#include "RigidBody.h"
#include <Core/SceneTransformComponents.h>
#include <Maths/Vector3.h>
#include "Collisions/CollisionSystem.h"
#include "Collisions/CollisionEvent.h"
#include <math.h>

#include "ForceHandler.h"
#include "Forces/Gravity.h"
#include "Forces/AirResistance.h"

#include "PhysicsWorld.h"
#include "PhysicsSystemConfig.h"
#include "Physics/Collisions/PhysicsCollisionResult.h"


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

RaycastingResult PhysicsSystem::CastRay(const Ray ray, const Entity ignoreEntity, const std::vector<uint8> layers) const
{
	return physicsWorld->CastRay(ray, layers, ignoreEntity);
}

std::vector<PhysicsCollisionCastResult> PhysicsSystem::CastBox(const CollisionAABB box, const std::vector<uint8> layers) const
{
	return physicsWorld->BoxCast(box, layers);
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
	const float MaxPhysicsFrameTime = .1f;

	deltaTime = fminf(deltaTime, MaxPhysicsFrameTime);

	world->ForEntities<RigidBodyComponent, SceneTransformComponent>
		(
			[&](const Entity entity, RigidBodyComponent* rigidBody, SceneTransformComponent* transform)
			{
				if (!transform->IsStatic())
				{
					Vector3 frameResultant = rigidBody->GetResultantForce();

					rigidBody->velocity += frameResultant * rigidBody->GetInverseMass() * deltaTime;
					transform->AddTranslation(rigidBody->velocity * deltaTime);
					rigidBody->ClearResultantForce();
				}

			}
		);
}
