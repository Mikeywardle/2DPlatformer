#include "PhysicsWorld.h"

#include <Core/World.h>

#include <Collisions/CollisionShapes.h>
#include <Collisions/CollisionDetection.h>

#include <Core/SceneTransformComponents.h>

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

	totalCollisionLayers = TotalCollisionLayers;
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

void PhysicsWorld::SetDynamicWorldLimits(const Vector3& position, const Vector3& limits)
{
	for (int i = 0; i < CollisionLayers.size(); ++i)
	{
		CollisionLayers[i]->SetDynamicLimits(position, limits);
	}
}

void PhysicsWorld::SetStaticWorldLimits(const Vector3& position, const Vector3& limits)
{
	for (int i = 0; i < CollisionLayers.size(); ++i)
	{
		CollisionLayers[i]->SetStaticLimits(position, limits);
	}
}

void PhysicsWorld::QueryCollider
(
	std::vector<PhysicsCollisionResult>& colliderResults
	,std::vector<PhysicsCollisionResult>& triggerResults
	, const PhysicsCollisionWorldData query
	, const ColliderGeometryComponent* collider
	, const SceneTransformComponent* transform
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

		//Avoid testing object against itself
		if (query.entity == broadPhaseCollider.entity)
			continue;

		result.entityA = query.entity;
		result.entityB = broadPhaseCollider.entity;

		const SceneTransformComponent* transformB = world->GetComponent<SceneTransformComponent>(result.entityB);
		const ColliderGeometryComponent* colliderB = world->GetComponent<ColliderGeometryComponent>(result.entityB);


		const int colliderFunctionIndex = GetColliderPairingIndex((int8)query.type, (int8)broadPhaseCollider.type);
		result.collisionInfo = CollisionFunctionsCallbacks[colliderFunctionIndex](collider, transform, colliderB, transformB);

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

		const SceneTransformComponent* transformB = world->GetComponent<SceneTransformComponent>(result.entityB);
		const ColliderGeometryComponent* colliderB = world->GetComponent<ColliderGeometryComponent>(result.entityB);

		const int colliderFunctionIndex = GetColliderPairingIndex((int8)query.type, (int8)broadPhaseCollider.type);
		result.collisionInfo = CollisionFunctionsCallbacks[colliderFunctionIndex](collider, transform, colliderB, transformB);

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

RaycastingResult PhysicsWorld::CastRay(const Ray ray, const std::vector<uint8> collisionLayers, const Entity toIgnoreEntity) const
{
	RaycastingResult result;

	if (collisionLayers.size() == 0)
	{
		//Test All layers
		for (int i = 0; i < totalCollisionLayers; ++i)
		{
			const RaycastingResult layerResult = CollisionLayers[i]->CastRay(ray, toIgnoreEntity, PhysicsWorldCollisionFunctions::RaycastingCallback, world);

			if (layerResult.hasHit)
			{
				if (!result.hasHit || layerResult.Distance < result.Distance)
				{
					result = layerResult;
				}
			}
		}
	}
	else
	{
		for (const uint8& layer : collisionLayers)
		{
			const RaycastingResult layerResult = CollisionLayers[layer]->CastRay(ray, toIgnoreEntity, PhysicsWorldCollisionFunctions::RaycastingCallback, world);


			if (layerResult.hasHit)
			{
				if (!result.hasHit || layerResult.Distance < result.Distance)
				{
					result = layerResult;
				}
			}
		}
	}


	return result;
}

std::vector<PhysicsCollisionCastResult> PhysicsWorld::BoxCast(const CollisionAABB box, std::vector<uint8> collisionLayers) const
{
	std::vector<PhysicsCollisionWorldData> BroadPhaseHits;
	if (collisionLayers.size() == 0)
	{
		//Test All layers
		for (int i = 0; i < totalCollisionLayers; ++i)
		{
			CollisionLayers[i]->CastBox(box, BroadPhaseHits);
		}
	}
	else
	{
		for (const uint8& layer : collisionLayers)
		{
			CollisionLayers[layer]->CastBox(box, BroadPhaseHits);
		}
	}

	std::vector<PhysicsCollisionCastResult> results;

	results.reserve(BroadPhaseHits.size());

	for (const PhysicsCollisionWorldData& item : BroadPhaseHits)
	{
		switch (item.type)
		{
		case ColliderType::AABB :
			{
			SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(item.entity);
				const AABBCollisionGeometry* boxcollider = world->GetComponent<ColliderGeometryComponent>(item.entity)->GetAABBGeometry();
				const CollisionAABB aabb = boxcollider->GetAABBLimits(transform);

				CollisionResult collisionResult = TestABBvAABB(aabb, box);

				if (collisionResult.hasCollision)
				{
					PhysicsCollisionCastResult result;
					result.collisionInfo = collisionResult;
					result.entity = item.entity;

					results.push_back(result);
				}
			}

			break;

		case ColliderType::Sphere:
			{
			SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(item.entity);
				const SphereCollisionGeometry* sphereCollider = world->GetComponent<ColliderGeometryComponent>(item.entity)->GetSphereGeometry();
				const CollisionSphere sphere = sphereCollider->GetCollisionSphere(transform);

				CollisionResult collisionResult = TestSphereVsAABB(sphere, box);

				if (collisionResult.hasCollision)
				{
					PhysicsCollisionCastResult result;
					result.collisionInfo = collisionResult;
					result.entity = item.entity;

					results.push_back(result);
				}
			}
			break;
		}
	}

	results.shrink_to_fit();

	return results;
}

PhysicsCollisionLayer* PhysicsWorld::GetCollisionLayer(int index) const
{
	return CollisionLayers[index];
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
	CollisionResult PhysicsWorldCollisionFunctions::AABBvsAABB(const ColliderGeometryComponent* colliderA, const SceneTransformComponent* transformA, const ColliderGeometryComponent* colliderB, const SceneTransformComponent* transformB)
	{
		const AABBCollisionGeometry* AABBColliderA = colliderA->GetAABBGeometry();
		const AABBCollisionGeometry* AABBColliderB = colliderB->GetAABBGeometry();

		const CollisionAABB boxA = AABBColliderA->GetAABBLimits(transformA);
		const CollisionAABB boxB = AABBColliderB->GetAABBLimits(transformB);

		return TestABBvAABB(boxA, boxB);
	}

	CollisionResult PhysicsWorldCollisionFunctions::AABBvsSphere(const ColliderGeometryComponent* colliderA, const SceneTransformComponent* transformA, const ColliderGeometryComponent* colliderB, const SceneTransformComponent* transformB)
	{
		const AABBCollisionGeometry* AABBColliderA = colliderA->GetAABBGeometry();
		const SphereCollisionGeometry* SphereColliderB = colliderB->GetSphereGeometry();

		const CollisionAABB box = AABBColliderA->GetAABBLimits(transformA);
		const CollisionSphere sphere = SphereColliderB->GetCollisionSphere(transformB);

		return FlipCollisionResult(TestSphereVsAABB(sphere, box));
	}

	CollisionResult PhysicsWorldCollisionFunctions::SpherevsAABB(const ColliderGeometryComponent* colliderA, const SceneTransformComponent* transformA, const ColliderGeometryComponent* colliderB, const SceneTransformComponent* transformB)
	{
		const SphereCollisionGeometry* SphereColliderA = colliderA->GetSphereGeometry();
		const AABBCollisionGeometry* AABBColliderB = colliderB->GetAABBGeometry();

		const CollisionSphere sphere = SphereColliderA->GetCollisionSphere(transformA);
		const CollisionAABB box = AABBColliderB->GetAABBLimits(transformB);

		return TestSphereVsAABB(sphere, box);
	}

	CollisionResult PhysicsWorldCollisionFunctions::SpherevsSphere(const ColliderGeometryComponent* colliderA, const SceneTransformComponent* transformA, const ColliderGeometryComponent* colliderB, const SceneTransformComponent* transformB)
	{
		const SphereCollisionGeometry* SphereColliderA = colliderA->GetSphereGeometry();
		const SphereCollisionGeometry* SphereColliderB = colliderB->GetSphereGeometry();

		const CollisionSphere sphereA = SphereColliderA->GetCollisionSphere(transformA);
		const CollisionSphere sphereB = SphereColliderB->GetCollisionSphere(transformB);

		return TestSpherevsSphere(sphereA, sphereB);
	}

	RaycastingResult RaycastingCallback(const Ray ray, const PhysicsCollisionWorldData item, const unsigned int IgnoreEntity, const void* userData)
	{
		if (item.entity == IgnoreEntity)
			return RaycastingResult();

		const World* world = (World*)userData;

		switch (item.type)
		{
		case ColliderType::AABB:

		{
			const SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(item.entity);
			const AABBCollisionGeometry* boxcollider = world->GetComponent<ColliderGeometryComponent>(item.entity)->GetAABBGeometry();
			const CollisionAABB aabb = boxcollider->GetAABBLimits(transform);

			RaycastingResult result = TestRayVsAABB(ray, aabb);
			result.HitEntity = item.entity;
			return result;
			break;
		}

		case::ColliderType::Sphere:
		{
			const SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(item.entity);
			const SphereCollisionGeometry* sphereCollider = world->GetComponent<ColliderGeometryComponent>(item.entity)->GetSphereGeometry();
			const CollisionSphere sphere = sphereCollider->GetCollisionSphere(transform);

			RaycastingResult result = TestRayVsSphere(ray, sphere);
			result.HitEntity = item.entity;

			return result;
			break;
		}


		default:
			return RaycastingResult();
			break;
		}

	}
}
