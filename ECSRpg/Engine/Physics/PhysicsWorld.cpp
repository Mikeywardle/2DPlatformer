#include "PhysicsWorld.h"

#include <Core/World.h>

#include <Collisions/CollisionQuadtree.h>
#include <Collisions/CollisionShapes.h>
#include <Collisions/CollisionDetection.h>

#include <Maths/Transform.h>

#include <Physics/PhysicsCollisionWorldData.h>
#include <Physics/Collisions/PhysicsCollisionResult.h>
#include <Physics/Collisions/CollisionComponents.h>
#include <Physics/PhysicsCollisionLayer.h>

PhysicsWorld::PhysicsWorld(World* InWorld, const uint8 TotalCollisionLayers)
{
	world = InWorld;

	CollisionFunctionsCallbacks =
	{
		//AABB vs
		&PhysicsWorldCollisionFunctions::AABBvsAABB
		,&PhysicsWorldCollisionFunctions::AABBvsSphere
		//Sphere vs
		,&PhysicsWorldCollisionFunctions::SpherevsAABB
		,&PhysicsWorldCollisionFunctions::SpherevsSphere
	};	

	for (int i = 0; i < TotalCollisionLayers; ++i)
	{
		CollisionLayers.push_back(new PhysicsCollisionLayer());
	}
}

PhysicsWorld::~PhysicsWorld()
{
	for(PhysicsCollisionLayer* layer : CollisionLayers)
	{
		delete(layer);
	}
}

void PhysicsWorld::ClearDynamicWorld()
{
	for (int i = 0; i < CollisionLayers.size(); ++i)
	{
		CollisionLayers[i]->ClearDynamicObjects();
	}
}

void PhysicsWorld::AddDynamicBody(const PhysicsCollisionWorldData& toAdd, const uint8 CollisionLayer)
{
	CollisionLayers[CollisionLayer]->AddDynamicBody(toAdd);
}

void PhysicsWorld::ClearStaticWorld()
{
	for (int i = 0; i < CollisionLayers.size(); ++i)
	{
		CollisionLayers[i]->ClearStaticObjects();
	}
}

void PhysicsWorld::AddStaticBody(const PhysicsCollisionWorldData& toAdd, const uint8 CollisionLayer)
{
	CollisionLayers[CollisionLayer]->AddStaticBody(toAdd);
}

void PhysicsWorld::SetDynamicWorldLimits(const Vector2& position, const Vector2& halfLimits)
{
	for (int i = 0; i < CollisionLayers.size(); ++i)
	{
		CollisionLayers[i]->SetDynamicLimits(position, halfLimits);
	}
}

void PhysicsWorld::SetStaticWorldLimits(const Vector2& position, const Vector2& halfLimits)
{
	for (int i = 0; i < CollisionLayers.size(); ++i)
	{
		CollisionLayers[i]->SetStaticLimits(position, halfLimits);
	}
}

void PhysicsWorld::QueryCollider
(
	std::vector<PhysicsCollisionResult>& colliderResults
	,std::vector<PhysicsCollisionResult>& triggerResults
	, const PhysicsCollisionWorldData query
	, const IColliderComponent* collider, Transform* transform
	, const std::vector<uint8>& collisionLayers
) const
{
	//BroadPhase Culling
	std::vector<PhysicsCollisionWorldData> broadPhaseResults;
	std::vector<PhysicsCollisionWorldData> broadPhaseTriggerResults;

	for (uint8 layer : collisionLayers)
	{
		CollisionLayers[layer]->QueryCollider(broadPhaseResults, broadPhaseTriggerResults, query);
	}

	//Narrow Phase get results
	//Colliders
	for (const PhysicsCollisionWorldData broadPhaseCollider : broadPhaseResults)
	{
		PhysicsCollisionResult result;

		result.entityA = query.entity;
		result.entityB = broadPhaseCollider.entity;

		const int colliderFunctionIndex = GetColliderPairingIndex((int8)query.type, (int8)broadPhaseCollider.type);
		result.collisionInfo = CollisionFunctionsCallbacks[colliderFunctionIndex](collider, transform, broadPhaseCollider, world);

		if (result.collisionInfo.hasCollision)
		{
			colliderResults.push_back(result);
		}
	}

	//Triggers
	for (const PhysicsCollisionWorldData broadPhaseCollider : broadPhaseTriggerResults)
	{
		PhysicsCollisionResult result;

		result.entityA = query.entity;
		result.entityB = broadPhaseCollider.entity;

		const int colliderFunctionIndex = GetColliderPairingIndex((int8)query.type, (int8)broadPhaseCollider.type);
		result.collisionInfo = CollisionFunctionsCallbacks[colliderFunctionIndex](collider, transform, broadPhaseCollider, world);

		if (result.collisionInfo.hasCollision)
		{
			triggerResults.push_back(result);
		}
	}

}

Vector3 PhysicsWorld::GetGravity() const
{
	return Vector3(0.0f, -9.81f, 0.0f);
}

int PhysicsWorld::GetColliderPairingIndex(const int8& a, const int8& b) const
{
	return b + ((int8)ColliderType::TotalCollisionComponents * a);
}

int PhysicsWorld::GetTotalNumberOfColliderPairings()
{
	return (int8)ColliderType::TotalCollisionComponents * (int8)ColliderType::TotalCollisionComponents;
}


namespace PhysicsWorldCollisionFunctions
{
	CollisionResult PhysicsWorldCollisionFunctions::AABBvsAABB(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world)
	{
		const AABBColliderComponent* AABBColliderA = (AABBColliderComponent*)colliderA;

		Transform* transformB = world->GetComponent<Transform>(entityB.entity);
		const AABBColliderComponent* AABBColliderB = world->GetComponent< AABBColliderComponent>(entityB.entity);

		const CollisionAABB boxA = AABBColliderA->GetAABBLimits(transformA);
		const CollisionAABB boxB = AABBColliderB->GetAABBLimits(transformB);

		return TestABBvAABB(boxA, boxB);
	}

	CollisionResult PhysicsWorldCollisionFunctions::AABBvsSphere(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world)
	{
		const AABBColliderComponent* AABBColliderA = (AABBColliderComponent*)colliderA;

		Transform* transformB = world->GetComponent<Transform>(entityB.entity);
		const SphereColliderComponent* SphereColliderB = world->GetComponent< SphereColliderComponent>(entityB.entity);

		const CollisionAABB box = AABBColliderA->GetAABBLimits(transformA);
		const CollisionSphere sphere = SphereColliderB->GetCollisionSphere(transformB);


		return FlipCollisionResult(TestSphereVsAABB(sphere, box));
	}

	CollisionResult PhysicsWorldCollisionFunctions::SpherevsAABB(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world)
	{
		const SphereColliderComponent* sphereColliderA = (SphereColliderComponent*)colliderA;

		Transform* transformB = world->GetComponent<Transform>(entityB.entity);
		const AABBColliderComponent* AABBColliderB = world->GetComponent< AABBColliderComponent>(entityB.entity);

		const CollisionSphere sphere = sphereColliderA->GetCollisionSphere(transformA);
		const CollisionAABB box = AABBColliderB->GetAABBLimits(transformB);

		return TestSphereVsAABB(sphere, box);
	}

	CollisionResult PhysicsWorldCollisionFunctions::SpherevsSphere(const IColliderComponent* colliderA, Transform* transformA, const PhysicsCollisionWorldData entityB, World* world)
	{
		const SphereColliderComponent* sphereColliderA = (SphereColliderComponent*) colliderA;

		Transform* transformB = world->GetComponent<Transform>(entityB.entity);
		const SphereColliderComponent* sphereColliderB = world->GetComponent< SphereColliderComponent>(entityB.entity);

		const CollisionSphere sphereA = sphereColliderA->GetCollisionSphere(transformA);
		const CollisionSphere sphereB = sphereColliderB->GetCollisionSphere(transformB);

		return TestSpherevsSphere(sphereA, sphereB);
	}
}
