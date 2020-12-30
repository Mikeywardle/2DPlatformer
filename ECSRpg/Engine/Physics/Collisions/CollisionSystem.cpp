#include "CollisionSystem.h"
#include "CollisionEvent.h"

CollisionSystem::CollisionSystem(World* world)
{
	this->world = world;
}

void CollisionSystem::ProcessCollisions()
{
	CollisionEvent collisionResults;

	TestSphereColliders(collisionResults.results);
	TestAABBColliders(collisionResults.results);

	if (collisionResults.results.size() > 0)
	{
		world->DispatchMessage<CollisionEvent>(collisionResults);
	}
}

void CollisionSystem::TestSphereColliders(std::vector<CollisionResult>& collisionResults) const
{
	std::vector<Entity> entities = world->GetEntities<SphereColliderComponent, Transform>();

	for (int i = 0; i<entities.size(); ++i)
	{
		Entity entityA = entities[i];
		Transform* transformA = world->GetComponent<Transform>(entityA);
		if (transformA->IsStatic())
			continue;

		SphereColliderComponent* colliderA= world->GetComponent<SphereColliderComponent>(entityA);
		CollisionSphere sphereA = CollisionSphere(entityA, colliderA->radius, transformA->GetPosition());

		for (int j = i + 1; j < entities.size(); ++j)
		{
			Entity entityB = entities[j];
			Transform* transformB = world->GetComponent<Transform>(entityB);
			SphereColliderComponent* colliderB = world->GetComponent<SphereColliderComponent>(entityB);
			CollisionSphere sphereB = CollisionSphere(entityB, colliderB->radius, transformB->GetPosition());

			CollisionResult result = TestSpherevsSphere(sphereA, sphereB);

			if (result.hasCollision)
				collisionResults.push_back(result);
		}

		QueryAABBsWithSphere<>(sphereA, collisionResults);
	}
}

void CollisionSystem::TestAABBColliders(std::vector<CollisionResult>& collisionResults) const
{
	std::vector<Entity> entities = world->GetEntities<AABBColliderComponent, Transform>();

	for (int i = 0; i < entities.size(); ++i)
	{
		Entity entityA = entities[i];
		Transform* transformA = world->GetComponent<Transform>(entityA);
		if (transformA->IsStatic())
			continue;

		AABBColliderComponent* colliderA = world->GetComponent<AABBColliderComponent>(entityA);

		CollisionAABB boxA = CollisionAABB(entityA,
			transformA->GetPosition(),
			colliderA->halfWidth,
			colliderA->halfHeight,
			colliderA->halfDepth);

		for (int j = i + 1; j < entities.size(); ++j)
		{
			Entity entityB = entities[j];
			Transform* transformB = world->GetComponent<Transform>(entityB);
			AABBColliderComponent* colliderB = world->GetComponent<AABBColliderComponent>(entityB);

			CollisionAABB boxB = CollisionAABB(entityB,
				transformB->GetPosition(),
				colliderB->halfWidth,
				colliderB->halfHeight,
				colliderB->halfDepth);

			CollisionResult result = TestABBvAABB(boxA, boxB);

			if (result.hasCollision)
				collisionResults.push_back(result);
		}


	}
}
