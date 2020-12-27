#include "PhysicsSystem.h"

#include "RigidBody.h"
#include <Core/Transform.h>
#include <Maths/Vector3.h>
#include "Collisions/CollisionSystem.h"
#include "Collisions/Collider.h"
#include "Collisions/CollisionEvent.h"
#include <math.h>

#include "ForceHandler.h"
#include "Forces/Gravity.h"
#include "Forces/AirResistance.h"


PhysicsSystem::PhysicsSystem(World* world)
{
	this->world = world;

	forceHandlers =
	{
		new GravityForceHandler(),
		new AirResistanceForceHandler()
	};

	this->collisionSystem = new CollisionSystem(world);

	world->AddListener<CollisionEvent>(this, &PhysicsSystem::ResolveCollisions);
}

void PhysicsSystem::ProcessPhysicsForFrame(float deltaTime)
{
	ProcessForceHandlers(deltaTime);
	IntegrateForces(deltaTime);

	collisionSystem->ProcessCollisions();
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

void PhysicsSystem::ResolveCollisions(CollisionEvent collisionResults)
{
	for (CollisionResult result : collisionResults.results)
	{
		Transform* ta = world->GetComponent<Transform>(result.entityA);
		Transform* tb = world->GetComponent<Transform>(result.entityB);

		RigidBodyComponent* rba = world->GetComponent<RigidBodyComponent>(result.entityA);
		RigidBodyComponent* rbb = world->GetComponent<RigidBodyComponent>(result.entityB);

		bool NoPhysicsComponents = rba == nullptr && rbb == nullptr; 
		const float TotalInverseMass = GetTotalInversMass(rba, rbb);

		//Resolve Interpentration
		const float depth = result.collisionDepth;
		Vector3 collisionNormal = result.normal;

		const bool aStatic = ta->IsStatic();
		const bool bStatic = tb->IsStatic();

		if (!aStatic && !bStatic && !NoPhysicsComponents)
		{
			Vector3 movePerImass = collisionNormal * (depth / TotalInverseMass);

			ta->AddTranslation(movePerImass * rba->GetInverseMass());
			tb->AddTranslation(movePerImass * -1.0 *rbb->GetInverseMass());
		}
		else if (!aStatic)
		{
			ta->AddTranslation(collisionNormal * depth);
		}
		else if (!bStatic)
		{
			tb->AddTranslation(collisionNormal * depth);
		}

		//Resolve Velocity
		if (NoPhysicsComponents || TotalInverseMass <= 0)
			continue;

		const Vector3 relativeVelocity = GetRelativeVelocity(rba, rbb);
		const float SeparatingVelocity = GetSeparatingVelocity(relativeVelocity, result.normal);

		if (SeparatingVelocity > 0 )
			continue;

		const float collisionRestitution = GetCollisionRestitution(rba, rbb);
		const float newSepVelocity = -SeparatingVelocity * collisionRestitution;
		const float deltaVelocity = newSepVelocity - SeparatingVelocity;

		const float impulse = deltaVelocity / TotalInverseMass;

		const Vector3 ImpulserPerIMass = result.normal * impulse;

		if (rba != nullptr)
			rba->velocity += ImpulserPerIMass * rba->GetInverseMass();

		if (rbb != nullptr)
			rbb->velocity -= ImpulserPerIMass * rbb->GetInverseMass();

		//Add Friction
		const float TotalFrictionCoefficient = GetTotalFrictionCoefficient(rba, rbb);
		const Vector3 tangent = Vector3::Normalize(relativeVelocity - (collisionNormal * Vector3::DotProduct(relativeVelocity, collisionNormal)));

		const float frictionMagnitude = -Vector3::DotProduct(relativeVelocity, tangent) / TotalInverseMass;

		const Vector3 frictionImpulse = tangent * frictionMagnitude * TotalFrictionCoefficient;

		if (rba != nullptr)
			rba->velocity += frictionImpulse * rba->GetInverseMass();

		if (rbb != nullptr)
			rbb->velocity -= frictionImpulse * rbb->GetInverseMass();
	}

}

float PhysicsSystem::GetSeparatingVelocity(Vector3 relativeVelocity, Vector3 Normal)
{
	return Vector3::DotProduct(relativeVelocity, Normal);
}

Vector3 PhysicsSystem::GetRelativeVelocity(RigidBodyComponent* rb1, RigidBodyComponent* rb2)
{
	if (rb2 == nullptr)
		return rb1->velocity;
	else if (rb1 == nullptr)
		return rb2->velocity * -1;
	else
		return rb1->velocity - rb2->velocity;
}

float PhysicsSystem::GetCollisionRestitution(RigidBodyComponent* rb1, RigidBodyComponent* rb2)
{
	if (rb2 == nullptr)
		return rb1->Restitution;
	else if (rb1 == nullptr)
		return rb2->Restitution;
	else
		return fmaxf(rb1->Restitution, rb2->Restitution);
}

float PhysicsSystem::GetTotalInversMass(RigidBodyComponent* rb1, RigidBodyComponent* rb2)
{
	if (rb2 == nullptr)
		return rb1->GetInverseMass();
	else if (rb1 == nullptr)
		return rb2->GetInverseMass();
	else
		return rb1->GetInverseMass() + rb2->GetInverseMass();
}

float PhysicsSystem::GetTotalFrictionCoefficient(RigidBodyComponent* rb1, RigidBodyComponent* rb2)
{
	if (rb1 == nullptr)
	{
		return rb2->Friction;
	}
	else if (rb2 == nullptr)
	{
		return rb1->Friction;
	}
	else
	{
		float fric1 = rb1->Friction;
		float fric2 = rb2->Friction;

		return sqrtf((fric1 * fric1) * (fric2 * fric2));
	}
}

void PhysicsSystem::ApplyFriction(RigidBodyComponent* rigidbody, Vector3 normal, float FrictionCoefficient)
{
	if (rigidbody == nullptr)
		return;

	Vector3 FrameForce = rigidbody->GetFrameForce();
	Vector3 FrameForceDir = Vector3::Normalize(FrameForce);

}
