#include "CollisionSystem.h"
#include "CollisionEvent.h"

#include <Maths/Transform.h>

#include <Physics/PhysicsWorld.h>
#include <Physics/RigidBody.h>
#include <Physics/PhysicsCollisionWorldData.h>

CollisionSystem::CollisionSystem(World* world)
{
	this->world = world;
}

void CollisionSystem::ProcessCollisions(PhysicsWorld* physWorld)
{
	CollisionEvent collisionResults;
	TriggerOverlapEvent triggerOverlapResults;

	FindDynamicCollisions(collisionResults.results, triggerOverlapResults.results,physWorld);
	ResolveCollisions(collisionResults.results);

	if (collisionResults.results.size() > 0)
	{
		world->DispatchMessage<CollisionEvent&>(collisionResults);
	}

	if (triggerOverlapResults.results.size() > 0)
	{
		world->DispatchMessage<TriggerOverlapEvent&>(triggerOverlapResults);
	}
}

void CollisionSystem::FindDynamicCollisions(std::vector<PhysicsCollisionResult>& colliderResults, std::vector<PhysicsCollisionResult>& triggerResults, PhysicsWorld* physWorld)
{
	//Clear previous colliders
	physWorld->ClearDynamicWorld();

	std::vector<Entity> entities = world->GetEntities<DynamicCollider, ColliderMetaComponent>();

	std::vector<std::vector<Entity>> sortedColliderEntities;

	//Calculate world limits
	float rightX, leftX, topY, bottomY;
	bool first = true;

	for (Entity entity : entities)
	{
		const ColliderMetaComponent* meta = world->GetComponent<ColliderMetaComponent>(entity);
		Transform* transform = world->GetComponent<Transform>(entity);
		const IColliderComponent* collider = GetEntityCollider(entity, meta);

		const CollisionAABB2D ColliderLimits = collider->GetAABB2DLimits(transform);

		const Vector3 position = transform->GetPosition();

		//Calculate Tree bounds
		if (first)
		{
			leftX = position.x - ColliderLimits.Position.x;
			rightX = position.x + ColliderLimits.Position.x;

			bottomY = position.z - ColliderLimits.Position.y;
			topY = position.z + ColliderLimits.Position.y;
			first = false;
		}
		else
		{
			leftX = fminf(leftX, position.x - ColliderLimits.Position.x);
			rightX = fmaxf(rightX, position.x + ColliderLimits.Position.x);

			bottomY = fminf(bottomY, position.z - ColliderLimits.Position.y);
			topY = fmaxf(topY, position.z + ColliderLimits.Position.y);
		}

		uint8 colliderLayer = meta->collisionLayer;

		if (sortedColliderEntities.size() <= colliderLayer)
		{
			sortedColliderEntities.resize(colliderLayer + 1);
		}

		sortedColliderEntities[colliderLayer].push_back(entity);
	}

	//Set World limits
	const Vector2 TreeHalfLimits = Vector2((rightX - leftX) / 2.0f, (topY - bottomY) / 2.0f);
	const Vector2 TreePosition = Vector2(leftX + TreeHalfLimits.x, bottomY + TreeHalfLimits.y);

	physWorld->SetDynamicWorldLimits(TreePosition, TreeHalfLimits);

	for (int i = 0; i < sortedColliderEntities.size(); ++i)
	{
		//For each collider, query current world and then add collider
		for (Entity entity : sortedColliderEntities[i])
		{
			const ColliderMetaComponent* meta = world->GetComponent<ColliderMetaComponent>(entity);
			Transform* transform = world->GetComponent<Transform>(entity);
			const IColliderComponent* collider = GetEntityCollider(entity, meta);

			//Create collider Data
			const PhysicsCollisionWorldData colliderData = PhysicsCollisionWorldData
			(
				entity
				, transform->GetPosition()
				, collider->GetAABBLimits(transform).HalfLimits
				, collider->GetColliderType()
				, meta->isTrigger
			);

			physWorld->QueryCollider(colliderResults, triggerResults, colliderData, collider, transform, meta->toCollideLayers);

			physWorld->AddDynamicBody(colliderData, meta->collisionLayer);
		}
	}

}

IColliderComponent* CollisionSystem::GetEntityCollider(const Entity entity, const ColliderMetaComponent* metaData)
{
	switch (metaData->type)
	{
	case ColliderType::AABB:
		return (IColliderComponent*)world->GetComponent<AABBColliderComponent>(entity);
		break;
	case ColliderType::Sphere:
		return (IColliderComponent*)world->GetComponent<SphereColliderComponent>(entity);
		break;
	default:
		return nullptr;
		break;
	}

}

void CollisionSystem::GenerateStaticColiisions(PhysicsWorld* physWorld)
{
	physWorld->ClearStaticWorld();

	std::vector<Entity> entities = world->GetEntities<StaticCollider, ColliderMetaComponent>();

	//Calculate world limits
	float rightX, leftX, topY, bottomY;
	bool first = true;

	for (Entity entity : entities)
	{
		const ColliderMetaComponent* meta = world->GetComponent<ColliderMetaComponent>(entity);
		Transform* transform = world->GetComponent<Transform>(entity);
		const IColliderComponent* collider = GetEntityCollider(entity, meta);

		const CollisionAABB2D ColliderLimits = collider->GetAABB2DLimits(transform);

		const Vector3 position = transform->GetPosition();

		if (first)
		{
			leftX = position.x - ColliderLimits.Position.x;
			rightX = position.x + ColliderLimits.Position.x;

			bottomY = position.z - ColliderLimits.Position.y;
			topY = position.z + ColliderLimits.Position.y;
			first = false;
		}
		else
		{
			leftX = fminf(leftX, position.x - ColliderLimits.Position.x);
			rightX = fmaxf(rightX, position.x + ColliderLimits.Position.x);

			bottomY = fminf(bottomY, position.z - ColliderLimits.Position.y);
			topY = fmaxf(topY, position.z + ColliderLimits.Position.y);
		}
	}

	const Vector2 TreeHalfLimits = Vector2((rightX - leftX) / 2.0f, (topY - bottomY) / 2.0f);
	const Vector2 TreePosition = Vector2(leftX + TreeHalfLimits.x, bottomY + TreeHalfLimits.y);

	physWorld->SetStaticWorldLimits(TreePosition, TreeHalfLimits);

	for (Entity entity : entities)
	{
		const ColliderMetaComponent* meta = world->GetComponent<ColliderMetaComponent>(entity);
		Transform* transform = world->GetComponent<Transform>(entity);
		const IColliderComponent* collider = GetEntityCollider(entity, meta);

		const PhysicsCollisionWorldData colliderData = PhysicsCollisionWorldData
		(
			entity
			, transform->GetPosition()
			, collider->GetAABBLimits(transform).HalfLimits
			, collider->GetColliderType()
			, meta->isTrigger
		);

		physWorld->AddStaticBody(colliderData, meta->collisionLayer);
	}
}

void CollisionSystem::ResolveCollisions(std::vector<PhysicsCollisionResult>& results)
{
	for (PhysicsCollisionResult result : results)
	{
		const CollisionResult resultInfo = result.collisionInfo;

		Transform* ta = world->GetComponent<Transform>(result.entityA);
		Transform* tb = world->GetComponent<Transform>(result.entityB);

		RigidBodyComponent* rba = world->GetComponent<RigidBodyComponent>(result.entityA);
		RigidBodyComponent* rbb = world->GetComponent<RigidBodyComponent>(result.entityB);

		bool NoPhysicsComponents = rba == nullptr && rbb == nullptr;
		const float TotalInverseMass = GetTotalInversMass(rba, rbb);

		//Resolve Interpentration
		const float depth = resultInfo.collisionDepth;
		Vector3 collisionNormal = resultInfo.normal;

		const bool aStatic = ta->IsStatic();
		const bool bStatic = tb->IsStatic();

		if (!aStatic && !bStatic && !NoPhysicsComponents)
		{
			Vector3 movePerImass = collisionNormal * (depth / TotalInverseMass);

			ta->AddTranslation(movePerImass * rba->GetInverseMass());
			tb->AddTranslation(movePerImass * -1.0 * rbb->GetInverseMass());
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
		const float SeparatingVelocity = GetSeparatingVelocity(relativeVelocity, resultInfo.normal);

		if (SeparatingVelocity > 0)
			continue;

		const float collisionRestitution = GetCollisionRestitution(rba, rbb);
		const float newSepVelocity = -SeparatingVelocity * collisionRestitution;
		const float deltaVelocity = newSepVelocity - SeparatingVelocity;

		const float impulse = deltaVelocity / TotalInverseMass;

		const Vector3 ImpulserPerIMass = resultInfo.normal * impulse;

		AddImpulseIfNotNull(rba, ImpulserPerIMass);
		AddImpulseIfNotNull(rbb, ImpulserPerIMass * -1);

		//Add Friction
		const float TotalFrictionCoefficient = GetTotalFrictionCoefficient(rba, rbb);
		const Vector3 tangent = Vector3::Normalize(relativeVelocity - (collisionNormal * Vector3::DotProduct(relativeVelocity, collisionNormal)));

		const float frictionMagnitude = -(Vector3::DotProduct(relativeVelocity, tangent) / TotalInverseMass);
		const float normalImpulseMagnitude = Vector3::Magnitude(ImpulserPerIMass);

		Vector3 frictionImpulse;
		if (fabsf(frictionMagnitude) < normalImpulseMagnitude * TotalFrictionCoefficient)
		{
			frictionImpulse = tangent * frictionMagnitude;
		}
		else
		{
			frictionImpulse = tangent * -normalImpulseMagnitude * TotalFrictionCoefficient;
		}

		AddImpulseIfNotNull(rba, frictionImpulse);
		AddImpulseIfNotNull(rbb, frictionImpulse * -1);
	}

}

void CollisionSystem::AddImpulseIfNotNull(RigidBodyComponent* rigidbody, const Vector3& frictionImpulse)
{
	if (rigidbody != nullptr)
		rigidbody->AddImpulse(frictionImpulse);
}

float CollisionSystem::GetSeparatingVelocity(Vector3 relativeVelocity, Vector3 Normal)
{
	return Vector3::DotProduct(relativeVelocity, Normal);
}

Vector3 CollisionSystem::GetRelativeVelocity(RigidBodyComponent* rb1, RigidBodyComponent* rb2)
{
	if (rb2 == nullptr)
		return rb1->velocity;
	else if (rb1 == nullptr)
		return rb2->velocity * -1;
	else
		return rb1->velocity - rb2->velocity;
}

float CollisionSystem::GetCollisionRestitution(RigidBodyComponent* rb1, RigidBodyComponent* rb2)
{
	if (rb2 == nullptr)
		return rb1->Restitution;
	else if (rb1 == nullptr)
		return rb2->Restitution;
	else
		return fmaxf(rb1->Restitution, rb2->Restitution);
}

float CollisionSystem::GetTotalInversMass(RigidBodyComponent* rb1, RigidBodyComponent* rb2)
{
	if (rb2 == nullptr)
		return rb1->GetInverseMass();
	else if (rb1 == nullptr)
		return rb2->GetInverseMass();
	else
		return rb1->GetInverseMass() + rb2->GetInverseMass();
}

float CollisionSystem::GetTotalFrictionCoefficient(RigidBodyComponent* rb1, RigidBodyComponent* rb2)
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

