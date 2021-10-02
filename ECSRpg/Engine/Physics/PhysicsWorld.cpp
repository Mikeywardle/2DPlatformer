#include "PhysicsWorld.h"

#include <Core/World.h>

#include <Collisions/CollisionQuadtree.h>
#include <Collisions/CollisionShapes.h>
#include <Collisions/CollisionDetection.h>

#include <Maths/Transform.h>

#include <Physics/PhysicsCollisionWorldData.h>
#include <Physics/Collisions/PhysicsCollisionResult.h>
#include <Physics/Collisions/CollisionComponents.h>

PhysicsWorld::PhysicsWorld(World* InWorld)
{
	world = InWorld;

	dynamicTree = new CollisionQuadtree<PhysicsCollisionWorldData>();
	staticTree = new CollisionQuadtree<PhysicsCollisionWorldData>();

	CollisionFunctionsCallbacks =
	{
		//AABB vs
		&PhysicsWorldCollisionFunctions::AABBvsAABB
		,&PhysicsWorldCollisionFunctions::AABBvsSphere
		//Sphere vs
		,&PhysicsWorldCollisionFunctions::SpherevsAABB
		,&PhysicsWorldCollisionFunctions::SpherevsSphere
	};
}

PhysicsWorld::~PhysicsWorld()
{
	delete(dynamicTree);
	delete(staticTree);
}

void PhysicsWorld::ClearDynamicWorld()
{
	dynamicTree->Clear();
}

void PhysicsWorld::AddDynamicBody(const PhysicsCollisionWorldData& toAdd)
{
	dynamicTree->Insert(toAdd);
}

void PhysicsWorld::ClearStaticWorld()
{
	staticTree->Clear();
}

void PhysicsWorld::AddStaticBody(const PhysicsCollisionWorldData& toAdd)
{
	staticTree->Insert(toAdd);
}

void PhysicsWorld::SetDynamicWorldLimits(const Vector2& position, const Vector2& halfLimits)
{
	dynamicTree->position = position;
	dynamicTree->halfLimits = halfLimits + Vector2(QuadTreeBorderWidth, QuadTreeBorderWidth);
}

void PhysicsWorld::SetStaticWorldLimits(const Vector2& position, const Vector2& halfLimits)
{
	staticTree->position = position;
	staticTree->halfLimits = halfLimits + Vector2(QuadTreeBorderWidth, QuadTreeBorderWidth);
}

void PhysicsWorld::QueryCollider(std::vector<PhysicsCollisionResult>& results, const PhysicsCollisionWorldData query, const IColliderComponent* collider, Transform* transform) const
{
	//BroadPhase Culling
	std::vector<PhysicsCollisionWorldData> broadPhaseResults;

	dynamicTree->Query(query, broadPhaseResults);
	staticTree->Query(query, broadPhaseResults);

	//Narrow Phase get results
	const CollisionAABB QueryAABB = collider->GetAABBLimits(transform);

	//TODO: refactor to handle multiple collider types
	for (const PhysicsCollisionWorldData broadPhaseCollider : broadPhaseResults)
	{
		const CollisionAABB colliderAABB = broadPhaseCollider.GetAABB();

		PhysicsCollisionResult result;

		result.entityA = query.entity;
		result.entityB = broadPhaseCollider.entity;

		const int colliderFunctionIndex = GetColliderPairingIndex((int8)query.type, (int8)broadPhaseCollider.type);
		result.collisionInfo = CollisionFunctionsCallbacks[colliderFunctionIndex](collider, transform, broadPhaseCollider, world);

		if (result.collisionInfo.hasCollision)
		{
			results.push_back(result);
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
