#include "PlayerWeaponSystem.h"

#include <Core/World.h>

#include <Inputs/InputData.h>
#include <Inputs/InputValues.h>

#include <Config/TestInputConfig.h>

#include <Components/WeaponComponents.h>

#include <Helpers/BulletCreationHelpers.h>

#include <Maths/Transform.h>

PlayerWeaponSystem::PlayerWeaponSystem(World* InWorld)
	: System(InWorld)
{
}

void PlayerWeaponSystem::OnInput(const float deltaTime, const InputData* inputData)
{
	bool firing = inputData->GetInputValue(TestConfigInputId::Fire, InputTypes::BUTTON_IS_DOWN);

	if (firing)
	{
		ForEntities(world, PlayerWeaponComponent, Transform)
		{
			PlayerWeaponComponent* pwc = world->GetComponent<PlayerWeaponComponent>(entity);

			pwc->timeToFire -= deltaTime;

			if (pwc->timeToFire <= 0)
			{
				const Transform* transform = world->GetComponent<Transform>(entity);
				BulletCreation::SpawnPlayerBullet(world, transform->GetPosition(), transform->GetForward());
				pwc->ResetTimeToFire();
			}
		}
	}
	else
	{
		std::vector<PlayerWeaponComponent>* weapons = world->GetComponents<PlayerWeaponComponent>();

		for (PlayerWeaponComponent& weapon : *weapons)
		{
			weapon.ResetTimeToFire();
		}
	}

}


