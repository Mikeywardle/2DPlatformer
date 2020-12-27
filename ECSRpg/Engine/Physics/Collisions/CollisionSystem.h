#pragma once

#include <Core/World.h>
#include "AABB.h"
#include "SphereCollider.h"
#include "Collider.h"
#include "CollisionDetection.h"

class CollisionSystem
{
public:
	CollisionSystem(class World* world);

	void ProcessCollisions();

#pragma region Shape Casting
	//Collider Casting
	template<typename ...Args>
	std::vector<Entity> CastSphere(struct Vector3 position, float radius)
	{
		std::vector<Entity> HitEntities;
		std::vector<CollisionResult> collisionResults;

		CollisionSphere sphere = CollisionSphere(NO_ENTITY, radius, position);

		QuerySpheresWithSphere<Args...>(sphere, collisionResults);
		QueryAABBsWithSphere<Args...>(sphere, collisionResults);

		for (CollisionResult result : collisionResults)
		{
			if (result.entityB == NO_ENTITY)
				HitEntities.push_back(result.entityA);
			else
				HitEntities.push_back(result.entityB);
		}

		return HitEntities;
	}

	template<typename ...Args>
	std::vector<Entity> CastBox(struct Vector3 position, float halfWidth, float halfHeight, float halfDepth)
	{
		std::vector<Entity> HitEntities;
		std::vector<CollisionResult> collisionResults;

		CollisionAABB box = CollisionAABB(NO_ENTITY, position, halfWidth, halfHeight, halfDepth);

		QueryAABBsWithAABB<Args...>(box, collisionResults);
		QuerySpheresWithAABB<Args...>(box, collisionResults);

		for (CollisionResult result : collisionResults)
		{
			if (result.entityB == NO_ENTITY)
				HitEntities.push_back(result.entityA);
			else
				HitEntities.push_back(result.entityB);
		}

		return HitEntities;
	}
#pragma endregion


private:
#pragma region Physical Collision Processing
	//TestShapes against world
	void TestSphereColliders(std::vector<CollisionResult>& collisionResults) const;
	void TestAABBColliders(std::vector<CollisionResult>& collisionResults) const;

#pragma endregion



#pragma region Templated Queries
	template<typename ...Args>
	void QuerySpheresWithSphere(CollisionSphere& inSphere, 
								std::vector<CollisionResult>& collisionResults, 
								Entity ignoreEntity = NO_ENTITY
	) const

	{
		std::vector<Entity> Colliders = world->GetEntities<Args..., SphereColliderComponent, Transform>();

		for (Entity entity : Colliders)
		{
			if (entity == ignoreEntity)
				continue;

			Transform* transform = world->GetComponent<Transform>(entity);
			SphereColliderComponent* collider = world->GetComponent<SphereColliderComponent>(entity);

			CollisionSphere sphere = CollisionSphere(entity, collider->radius, transform->GetPosition());

			CollisionResult result = TestSpherevsSphere(inSphere, sphere);

			if (result.hasCollision)
				collisionResults.push_back(result);
		}
	}

	template<typename ...Args>
	void QuerySpheresWithAABB(CollisionAABB& inBox,
						      std::vector<CollisionResult>& collisionResults,
							   Entity ignoreEntity = NO_ENTITY
	) const
	{
		std::vector<Entity> Colliders = world->GetEntities<Args..., SphereColliderComponent, Transform>();

		for (Entity entity : Colliders)
		{
			if (entity == ignoreEntity)
				continue;

			Transform* transform = world->GetComponent<Transform>(entity);
			SphereColliderComponent* collider = world->GetComponent<SphereColliderComponent>(entity);

			CollisionSphere sphere = CollisionSphere(entity, collider->radius, transform->GetPosition());

			CollisionResult result = TestSphereVsAABB(sphere, inBox);

			if (result.hasCollision)
				collisionResults.push_back(result);
		}
	}

	template<typename ...Args>
	void QueryAABBsWithSphere(CollisionSphere& inSphere,
		std::vector<CollisionResult>& collisionResults,
		Entity ignoreEntity = NO_ENTITY
	) const
	{
		std::vector<Entity> Colliders = world->GetEntities<Args..., AABBColliderComponent, Transform>();

		for (Entity entity : Colliders)
		{
			if (entity == ignoreEntity)
				continue;

			Transform* transform = world->GetComponent<Transform>(entity);
			AABBColliderComponent* collider = world->GetComponent<AABBColliderComponent>(entity);


			CollisionAABB box = CollisionAABB(entity,
				transform->GetPosition(),
				collider->halfWidth,
				collider->halfHeight,
				collider->halfDepth);

			CollisionResult result = TestSphereVsAABB(inSphere, box);

			if (result.hasCollision)
				collisionResults.push_back(result);
		}
	}

	template<typename ...Args>
	void QueryAABBsWithAABB(CollisionAABB & inBox,
							std::vector<CollisionResult> & collisionResults,
							Entity ignoreEntity = NO_ENTITY
	) const
	{
		std::vector<Entity> Colliders = world->GetEntities<Args..., AABBColliderComponent, Transform>();

		for (Entity entity : Colliders)
		{
			if (entity == ignoreEntity)
				continue;

			Transform* transform = world->GetComponent<Transform>(entity);
			AABBColliderComponent* collider = world->GetComponent<AABBColliderComponent>(entity);


			CollisionAABB box = CollisionAABB(entity,
				transform->GetPosition(),
				collider->halfWidth,
				collider->halfHeight,
				collider->halfDepth);

			CollisionResult result = TestABBvAABB(inBox, box);

			if (result.hasCollision)
				collisionResults.push_back(result);
		}
	}

#pragma endregion


	World* world;
};
