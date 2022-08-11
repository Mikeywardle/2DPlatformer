#include "PhysicsCollisionLayer.h"

#include "PhysicsCollisionWorldData.h"

#include <Collisions/CollisionGrid.h>
#include <Maths/Vector3.h>

#include <Core/World.h>


const float PhysicsCollisionLayer::BorderWidth = 2.f;
const Vector3 PhysicsCollisionLayer::CellSize = Vector3(5.0f, 5.0f, 5.0f);

PhysicsCollisionLayer::PhysicsCollisionLayer()
{
	dynamicColliders = new CollisionGrid<PhysicsCollisionWorldData>();
	dynamicTriggers = new CollisionGrid<PhysicsCollisionWorldData>();

	staticColliders = new CollisionGrid<PhysicsCollisionWorldData>();
	staticTriggers = new CollisionGrid<PhysicsCollisionWorldData>();
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

void PhysicsCollisionLayer::SetDynamicLimits(const Vector3& position, const Vector3& limits)
{
	dynamicColliders->SetGridDimensions(position, limits, CellSize);
	dynamicTriggers->SetGridDimensions(position , limits, CellSize);
}

void PhysicsCollisionLayer::SetStaticLimits(const Vector3& position, const Vector3& limits)
{

	staticColliders->SetGridDimensions(position, limits, CellSize);
	staticTriggers->SetGridDimensions(position, limits, CellSize);
}

void PhysicsCollisionLayer::SetLimits(const int& layerType, const Vector3& position, const Vector3& limits)
{

	switch (layerType)
	{
	case 0:
		dynamicColliders->SetGridDimensions(position, limits, CellSize);
		break;
	case 1:
		dynamicTriggers->SetGridDimensions(position, limits, CellSize);
		break;
	case 2:
		staticColliders->SetGridDimensions(position, limits, CellSize);
		break;
	case 3:
		staticTriggers->SetGridDimensions(position, limits, CellSize);
		break;
	}
}

void PhysicsCollisionLayer::SetDataArrayLength(const int& layerType, const int& length)
{
	switch (layerType)
	{
	case 0:
		dynamicColliders->SetDataArrayLength(length);
		break;
	case 1:
		dynamicTriggers->SetDataArrayLength(length);
		break;
	case 2:
		staticColliders->SetDataArrayLength(length);
		break;
	case 3:
		staticTriggers->SetDataArrayLength(length);
		break;
	}
}

void PhysicsCollisionLayer::BuildGridCells(const int& layerType)
{
	switch (layerType)
	{
	case 0:
		dynamicColliders->Build();
		break;
	case 1:
		dynamicTriggers->Build();
		break;
	case 2:
		staticColliders->Build();
		break;
	case 3:
		staticTriggers->Build();
		break;
	}
}

void PhysicsCollisionLayer::QueryCollider
(
	std::vector<PhysicsCollisionWorldData>& colliderOverlapResults
	, std::vector<PhysicsCollisionWorldData>& triggerOverlapResults
	, const PhysicsCollisionWorldData query
) const
{
	const CollisionAABB queryAABB = query.GetAABB();

	//If is trigger only test against colliders (as trigger/trigger interactions are not valid)
	if (query.IsTrigger)
	{
		dynamicColliders->Query(queryAABB, triggerOverlapResults);
		staticColliders->Query(queryAABB, triggerOverlapResults);
	}
	else
	{
		dynamicColliders->Query(queryAABB, colliderOverlapResults);
		staticColliders->Query(queryAABB, colliderOverlapResults);

		dynamicTriggers->Query(queryAABB, triggerOverlapResults);
		staticTriggers->Query(queryAABB, triggerOverlapResults);
	}

}

RaycastingResult PhysicsCollisionLayer::CastRay(const Ray ray, const unsigned int IgnoreEntity, PhysicsRaycastingCallback callback, const World* world) const
{
	const RaycastingResult dynamicResult = dynamicColliders->CastRay(ray, IgnoreEntity, callback, world);
	const RaycastingResult staticResult  = staticColliders->CastRay(ray, IgnoreEntity, callback, world);

	if (staticResult.hasHit && dynamicResult.hasHit)
	{
		if (staticResult.Distance < dynamicResult.Distance)
			return staticResult;
		else
			return dynamicResult;
	}
	else if(staticResult.hasHit)
	{
		return staticResult;
	}
	else
	{
		return dynamicResult;
	}
}

void PhysicsCollisionLayer::CastBox(const CollisionAABB box, std::vector<PhysicsCollisionWorldData>& results)
{
	dynamicColliders->Query(box, results);
	staticColliders->Query(box, results);
}
