#include "BulletSystem.h"

#include <Core/World.h>

#include <Components/BulletTags.h>
#include <Components/PlayerTags.h>
#include <Components/EnemyComponents.h>

#include <Maths/Transform.h>

#include <Physics/Collisions/CollisionEvent.h>

#include <GamePlay/Health.h>

BulletSystem::BulletSystem(World* world)
	:System(world)
{
	world->AddListener<TriggerOverlapEvent&>(this, &BulletSystem::OnBulletOverlap);
}

void BulletSystem::OnBulletOverlap(TriggerOverlapEvent& triggerOverlaps)
{
	for (PhysicsCollisionResult result : triggerOverlaps.results)
	{
		Entity bulletEntity;
		Entity targetEntity;

		bool entityIsbullet = CheckEntityIsBullet(result, bulletEntity, targetEntity);

		if (entityIsbullet)
		{
			if (world->HasComponent<HealthComponent>(targetEntity) && CheckBulletAndTargetCompatible(bulletEntity, targetEntity))
			{
				HealthComponent* health = world->GetComponent<HealthComponent>(targetEntity);
				const BulletComponent* bullet = world->GetComponent<BulletComponent>(bulletEntity);

				health->DealDamage(bullet->damage);

				if (health->Health <= 0)
				{
					world->DestroyEntity(targetEntity);
				}
			}

			world->DestroyEntity(bulletEntity);
		}
	}
}

bool BulletSystem::CheckEntityIsBullet(PhysicsCollisionResult result, Entity& bulletEntity, Entity& targetEntity) const
{

	if (world->HasComponent<BulletComponent>(result.entityA))
	{
		bulletEntity = result.entityA;
		targetEntity = result.entityB;

		return true;
	}
	else if (world->HasComponent<BulletComponent>(result.entityB))
	{
		bulletEntity = result.entityB;
		targetEntity = result.entityA;

		return true;
	}

	return false;
}

bool BulletSystem::CheckBulletAndTargetCompatible(const Entity bulletEntity, const Entity targetEntity) const
{
	const bool isPlayerBulletAndEnemy = world->HasComponent<PlayerBullet>(bulletEntity) && world->HasComponent<EnemyTag>(targetEntity);
	const bool isEnemyBulletAndPlayer = world->HasComponent<EnemyBullet>(bulletEntity) && world->HasComponent<PlayerTag>(targetEntity);

	return isPlayerBulletAndEnemy || isEnemyBulletAndPlayer;
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
