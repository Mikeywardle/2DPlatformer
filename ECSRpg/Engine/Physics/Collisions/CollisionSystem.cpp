#include "CollisionSystem.h"
#include "CollisionEvent.h"

#include <Maths/Transform.h>

#include <Physics/PhysicsWorld.h>
#include <Physics/RigidBody.h>
#include <Physics/PhysicsCollisionWorldData.h>

#include <Physics/PhysicsCollisionLayer.h>

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

	std::vector<Entity> entities = world->GetEntities<DynamicCollider, ColliderGeometryComponent>();

	std::vector<std::vector<PhysicsCollisionWorldData>> sortedColliderEntities;
	std::vector<std::vector<PhysicsCollisionWorldData>> sortedTriggerEntities;

	std::vector<CollisionWorldBoundariesData> sortedColliderBoundaries;
	std::vector<CollisionWorldBoundariesData> sortedTriggerBoundaries;

	const int NumberOfCollisionLayers = physWorld->GetTotalCollisionLayers();

	for (int i = 0; i < NumberOfCollisionLayers; ++i)
	{
		sortedColliderEntities.push_back(std::vector<PhysicsCollisionWorldData>());

		sortedTriggerEntities.push_back(std::vector<PhysicsCollisionWorldData>());

		sortedColliderBoundaries.push_back(CollisionWorldBoundariesData());
		sortedTriggerBoundaries.push_back(CollisionWorldBoundariesData());
	}	

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		const ColliderGeometryComponent* collider = world->GetComponent<ColliderGeometryComponent>(entity);

		const CollisionAABB ColliderLimits = collider->GetAABBLimits(transform);

		const Vector3 position = transform->GetPosition();

		const uint8 colliderLayer = collider->collisionLayer;

		const PhysicsCollisionWorldData colliderData = PhysicsCollisionWorldData
		(
			entity
			, position
			, collider->GetAABBLimits(transform).HalfLimits
			, collider->GetColliderType()
			, collider->isTrigger
		);

		CollisionWorldBoundariesData* bounds;

		if (colliderData.IsTrigger)
		{
			bounds = &sortedTriggerBoundaries[colliderLayer];
		}
		else
		{
			bounds = &sortedColliderBoundaries[colliderLayer];
		}

		//Calculate Tree bounds
		if (bounds->first)
		{

			bounds->bottomLeft = position - ColliderLimits.HalfLimits;
			bounds->topRight = position + ColliderLimits.HalfLimits;

			bounds->first = false;
		}
		else
		{
			bounds->bottomLeft.x = fminf(bounds->bottomLeft.x, position.x - ColliderLimits.HalfLimits.x);
			bounds->topRight.x = fmaxf(bounds->topRight.x, position.x + ColliderLimits.HalfLimits.x);

			bounds->bottomLeft.y = fminf(bounds->bottomLeft.y, position.y - ColliderLimits.HalfLimits.y);
			bounds->topRight.y = fmaxf(bounds->topRight.y, position.y + ColliderLimits.HalfLimits.y);

			bounds->bottomLeft.z = fminf(bounds->bottomLeft.z, position.z - ColliderLimits.HalfLimits.z);
			bounds->topRight.z = fmaxf(bounds->topRight.z, position.z + ColliderLimits.HalfLimits.z);
		}

		sortedColliderEntities[colliderLayer].push_back(colliderData);
	}

	//Set World limits
	for (int i = 0; i < NumberOfCollisionLayers; ++i)
	{
		PhysicsCollisionLayer* layer = physWorld->GetCollisionLayer(i);

		layer->SetDataArrayLength(0, sortedColliderEntities[i].size());
		layer->SetDataArrayLength(1, sortedTriggerEntities[i].size());

		const CollisionWorldBoundariesData& colliderData = sortedColliderBoundaries[i];
		const CollisionWorldBoundariesData& triggerData = sortedTriggerBoundaries[i];

		const Vector3 colliderGridSize = Vector3((colliderData.topRight.x - colliderData.bottomLeft.x), (colliderData.topRight.y - colliderData.bottomLeft.y), (colliderData.topRight.z - colliderData.bottomLeft.z));
		const Vector3 colliderPosition = colliderData.bottomLeft;
		layer->SetLimits(0, colliderPosition, colliderGridSize);

		const Vector3 triggerGridSize = Vector3((triggerData.topRight.x - triggerData.bottomLeft.x), (triggerData.topRight.y - triggerData.bottomLeft.y), (triggerData.topRight.z - triggerData.bottomLeft.z));
		const Vector3 triggerPosition = triggerData.bottomLeft;
		layer->SetLimits(1, triggerPosition, triggerGridSize);
	}

	//Add each item
	for (int i = 0; i < NumberOfCollisionLayers; ++i)
	{
		for (const PhysicsCollisionWorldData& data : sortedColliderEntities[i])
		{
			physWorld->AddDynamicBody(data, i);
		}

		for (const PhysicsCollisionWorldData& data : sortedTriggerEntities[i])
		{
			physWorld->AddDynamicBody(data, i);
		}

		PhysicsCollisionLayer* layer = physWorld->GetCollisionLayer(i);
		layer->BuildGridCells(0);
		layer->BuildGridCells(1);
	}

	for (int i = 0; i < NumberOfCollisionLayers; ++i)
	{
		for (const PhysicsCollisionWorldData& data : sortedColliderEntities[i])
		{
			const ColliderGeometryComponent* collider = world->GetComponent<ColliderGeometryComponent>(data.entity);
			Transform* transform = world->GetComponent<Transform>(data.entity);

			physWorld->QueryCollider(colliderResults, triggerResults, data, collider, transform, collider->toCollideLayers);
		}

		for (const PhysicsCollisionWorldData& data : sortedTriggerEntities[i])
		{
			const ColliderGeometryComponent* collider = world->GetComponent<ColliderGeometryComponent>(data.entity);
			Transform* transform = world->GetComponent<Transform>(data.entity);

			physWorld->QueryCollider(colliderResults, triggerResults, data, collider, transform, collider->toCollideLayers);
		}
	}

}

void CollisionSystem::GenerateStaticColiisions(PhysicsWorld* physWorld)
{
	//Clear previous colliders
	physWorld->ClearStaticWorld();

	std::vector<Entity> entities = world->GetEntities<StaticCollider, ColliderGeometryComponent>();

	std::vector<std::vector<PhysicsCollisionWorldData>> sortedColliderEntities;
	std::vector<std::vector<PhysicsCollisionWorldData>> sortedTriggerEntities;

	std::vector<CollisionWorldBoundariesData> sortedColliderBoundaries;
	std::vector<CollisionWorldBoundariesData> sortedTriggerBoundaries;

	const int NumberOfCollisionLayers = physWorld->GetTotalCollisionLayers();

	for (int i = 0; i < NumberOfCollisionLayers; ++i)
	{
		sortedColliderEntities.push_back(std::vector<PhysicsCollisionWorldData>());

		sortedTriggerEntities.push_back(std::vector<PhysicsCollisionWorldData>());

		sortedColliderBoundaries.push_back(CollisionWorldBoundariesData());
		sortedTriggerBoundaries.push_back(CollisionWorldBoundariesData());
	}

	for (Entity entity : entities)
	{
		const Transform* transform = world->GetComponent<Transform>(entity);
		const ColliderGeometryComponent* collider = world->GetComponent<ColliderGeometryComponent>(entity);

		const CollisionAABB ColliderLimits = collider->GetAABBLimits(transform);

		const Vector3 position = transform->GetPosition();

		const uint8 colliderLayer = collider->collisionLayer;

		const PhysicsCollisionWorldData colliderData = PhysicsCollisionWorldData
		(
			entity
			, position
			, collider->GetAABBLimits(transform).HalfLimits
			, collider->GetColliderType()
			, collider->isTrigger
		);

		CollisionWorldBoundariesData* bounds;

		if (colliderData.IsTrigger)
		{
			bounds = &sortedTriggerBoundaries[colliderLayer];
		}
		else
		{
			bounds = &sortedColliderBoundaries[colliderLayer];
		}

		//Calculate Tree bounds
		if (bounds->first)
		{

			bounds->bottomLeft = position - ColliderLimits.HalfLimits;
			bounds->topRight = position + ColliderLimits.HalfLimits;

			bounds->first = false;
		}
		else
		{
			bounds->bottomLeft.x = fminf(bounds->bottomLeft.x, position.x - ColliderLimits.HalfLimits.x);
			bounds->topRight.x = fmaxf(bounds->topRight.x, position.x + ColliderLimits.HalfLimits.x);

			bounds->bottomLeft.y = fminf(bounds->bottomLeft.y, position.y - ColliderLimits.HalfLimits.y);
			bounds->topRight.y = fmaxf(bounds->topRight.y, position.y + ColliderLimits.HalfLimits.y);

			bounds->bottomLeft.z = fminf(bounds->bottomLeft.z, position.z - ColliderLimits.HalfLimits.z);
			bounds->topRight.z = fmaxf(bounds->topRight.z, position.z + ColliderLimits.HalfLimits.z);
		}

		sortedColliderEntities[colliderLayer].push_back(colliderData);
	}

	//Set World limits
	for (int i = 0; i < NumberOfCollisionLayers; ++i)
	{
		PhysicsCollisionLayer* layer = physWorld->GetCollisionLayer(i);

		layer->SetDataArrayLength(2, sortedColliderEntities[i].size());
		layer->SetDataArrayLength(3, sortedTriggerEntities[i].size());

		const CollisionWorldBoundariesData& colliderData = sortedColliderBoundaries[i];
		const CollisionWorldBoundariesData& triggerData = sortedTriggerBoundaries[i];

		const Vector3 colliderGridSize = Vector3((colliderData.topRight.x - colliderData.bottomLeft.x), (colliderData.topRight.y - colliderData.bottomLeft.y), (colliderData.topRight.z - colliderData.bottomLeft.z));
		const Vector3 colliderPosition = colliderData.bottomLeft;
		layer->SetLimits(2, colliderPosition, colliderGridSize);

		const Vector3 triggerGridSize = Vector3((triggerData.topRight.x - triggerData.bottomLeft.x), (triggerData.topRight.y - triggerData.bottomLeft.y), (triggerData.topRight.z - triggerData.bottomLeft.z));
		const Vector3 triggerPosition = triggerData.bottomLeft;
		layer->SetLimits(3, triggerPosition, triggerGridSize);
	}

	//Add each item
	for (int i = 0; i < NumberOfCollisionLayers; ++i)
	{
		for (const PhysicsCollisionWorldData& data : sortedColliderEntities[i])
		{
			physWorld->AddStaticBody(data, i);
		}

		for (const PhysicsCollisionWorldData& data : sortedTriggerEntities[i])
		{
			physWorld->AddStaticBody(data, i);
		}

		PhysicsCollisionLayer* layer = physWorld->GetCollisionLayer(i);

		layer->BuildGridCells(2);
		layer->BuildGridCells(3);
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
		if (rba->frictionAllowed && rbb->frictionAllowed)
		{
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

