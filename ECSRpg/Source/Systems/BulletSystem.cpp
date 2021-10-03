#include "BulletSystem.h"

#include <Core/World.h>

#include <Components/BulletTags.h>
#include <Maths/Transform.h>

#include <Physics/Collisions/CollisionEvent.h>

BulletSystem::BulletSystem(World* world)
	:System(world)
{
	world->AddListener<TriggerOverlapEvent&>(this, &BulletSystem::OnBulletOverlap);

}

void BulletSystem::OnBulletOverlap(TriggerOverlapEvent& triggerOverlaps)
{
	for (PhysicsCollisionResult result : triggerOverlaps.results)
	{
		if (world->HasComponent<BulletMovementComponent>(result.entityA))
		{
			world->DestroyEntity(result.entityA);
		}
		else if (world->HasComponent<BulletMovementComponent>(result.entityB))
		{
			world->DestroyEntity(result.entityB);
		}
	}
}

void BulletSystem::OnFrame(const float deltaTime)
{
	ForEntities(world, BulletMovementComponent, Transform)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		const BulletMovementComponent* bmc = world->GetComponent<BulletMovementComponent>(entity);

		transform->AddTranslation(bmc->velocity * deltaTime);
	}

}
