#include "PhysicsCollisionLayer.h"

#include "PhysicsCollisionWorldData.h"

#include <Collisions/CollisionQuadtree.h>
#include <Maths/Vector2.h>


const float PhysicsCollisionLayer::QuadTreeBorderWidth = 10.f;

PhysicsCollisionLayer::PhysicsCollisionLayer()
{
	dynamicColliders = new CollisionQuadtree<PhysicsCollisionWorldData>();
	dynamicTriggers = new CollisionQuadtree<PhysicsCollisionWorldData>();

	staticColliders = new CollisionQuadtree<PhysicsCollisionWorldData>();
	staticTriggers = new CollisionQuadtree<PhysicsCollisionWorldData>();
}

PhysicsCollisionLayer::~PhysicsCollisionLayer()
{
	delete(dynamicColliders);
	delete(dynamicTriggers);

	delete(staticColliders);
	delete(staticTriggers);
}

void PhysicsCollisionLayer::ClearDynamicObjects()
{
	dynamicColliders->Clear();
	dynamicTriggers->Clear();
}

void PhysicsCollisionLayer::ClearStaticObjects()
{
	staticColliders->Clear();
	staticTriggers->Clear();
}

void PhysicsCollisionLayer::AddDynamicBody(const PhysicsCollisionWorldData& toAdd)
{
	if (toAdd.IsTrigger)
	{
		dynamicTriggers->Insert(toAdd);
	}
	else
	{
		dynamicColliders->Insert(toAdd);
	}
}

void PhysicsCollisionLayer::AddStaticBody(const PhysicsCollisionWorldData& toAdd)
{
	if (toAdd.IsTrigger)
	{
		staticTriggers->Insert(toAdd);
	}
	else
	{
		staticColliders->Insert(toAdd);
	}
}

void PhysicsCollisionLayer::SetDynamicLimits(const Vector2& position, const Vector2& halfLimits)
{
	dynamicColliders->position = position;
	dynamicColliders->halfLimits = halfLimits + Vector2(QuadTreeBorderWidth, QuadTreeBorderWidth);

	dynamicTriggers->position = position;
	dynamicTriggers->halfLimits = halfLimits + Vector2(QuadTreeBorderWidth, QuadTreeBorderWidth);
}

void PhysicsCollisionLayer::SetStaticLimits(const Vector2& position, const Vector2& halfLimits)
{
	staticColliders->position = position;
	staticColliders->halfLimits = halfLimits + Vector2(QuadTreeBorderWidth, QuadTreeBorderWidth);

	staticTriggers->position = position;
	staticTriggers->halfLimits = halfLimits + Vector2(QuadTreeBorderWidth, QuadTreeBorderWidth);
}

void PhysicsCollisionLayer::QueryCollider
(
	std::vector<PhysicsCollisionWorldData>& colliderOverlapResults
	, std::vector<PhysicsCollisionWorldData>& triggerOverlapResults
	, const PhysicsCollisionWorldData query
) const
{
	//If is trigger only test against colliders (as trigger/trigger interactions are not valid)
	if (query.IsTrigger)
	{
		dynamicColliders->Query(query, triggerOverlapResults);
		staticColliders->Query(query, triggerOverlapResults);
	}
	else
	{
		dynamicColliders->Query(query, colliderOverlapResults);
		staticColliders->Query(query, colliderOverlapResults);

		dynamicTriggers->Query(query, triggerOverlapResults);
		staticTriggers->Query(query, triggerOverlapResults);
	}

}
