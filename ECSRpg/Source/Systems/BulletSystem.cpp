#include "BulletSystem.h"

#include <Core/World.h>

#include <Components/BulletTags.h>
#include <Maths/Transform.h>

BulletSystem::BulletSystem(World* world)
	:System(world)
{
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
