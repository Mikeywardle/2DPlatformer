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

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		if (transform->IsStatic())
			continue;

		SphereColliderComponent* collider= world->GetComponent<SphereColliderComponent>(entity);
		CollisionSphere sphere = CollisionSphere(entity, collider->radius, transform->GetPosition());

		QuerySpheresWithSphere<>(sphere, collisionResults, entity);
		QueryAABBsWithSphere<>(sphere, collisionResults, entity);
	}
}

void CollisionSystem::TestAABBColliders(std::vector<CollisionResult>& collisionResults) const
{
	std::vector<Entity> entities = world->GetEntities<AABBColliderComponent, Transform>();

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		if (transform->IsStatic())
			continue;

		AABBColliderComponent* collider = world->GetComponent<AABBColliderComponent>(entity);

		CollisionAABB box = CollisionAABB(entity,
			transform->GetPosition(),
			collider->halfWidth,
			collider->halfHeight,
			collider->halfDepth);


		QueryAABBsWithAABB<>(box, collisionResults, entity);
	}
}
