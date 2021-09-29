#include "PhysicsWorld.h"

#include <Collisions/CollisionQuadtree.h>
#include <Physics/PhysicsCollisionWorldData.h>

#include <Maths/Transform.h>
#include <Collisions/CollisionShapes.h>
#include <Collisions/CollisionDetection.h>
#include <Physics/Collisions/PhysicsCollisionResult.h>

PhysicsWorld::PhysicsWorld(World* InWorld)
{
	world = InWorld;

	dynamicTree = new CollisionQuadtree<PhysicsCollisionWorldData>();
	staticTree = new CollisionQuadtree<PhysicsCollisionWorldData>();
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
	dynamicTree->halfLimits = halfLimits;
}

void PhysicsWorld::SetStaticWorldLimits(const Vector2& position, const Vector2& halfLimits)
{
	staticTree->position = position;
	staticTree->halfLimits = halfLimits;
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
	for (const PhysicsCollisionWorldData collider : broadPhaseResults)
	{
		const CollisionAABB colliderAABB = collider.GetAABB();

		PhysicsCollisionResult result;

		result.entityA = query.entity;
		result.entityB = collider.entity;
		result.collisionInfo = TestABBvAABB(QueryAABB, colliderAABB);

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

int PhysicsWorld::GetColliderPairingIndex(const int8& a, const int8& b)
{
	return b + ((int8)ColliderType::TotalCollisionComponents * a);
}

int PhysicsWorld::GetTotalNumberOfColliderPairings()
{
	return (int8)ColliderType::TotalCollisionComponents * (int8)ColliderType::TotalCollisionComponents;
}