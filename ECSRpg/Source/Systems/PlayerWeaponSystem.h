#pragma once

#include <ecs/System.h>

class PlayerWeaponSystem final : public System
{
public:

	PlayerWeaponSystem(World* InWorld);
	virtual void OnInput(const float deltaTime, const InputData* inputData) override;
};
