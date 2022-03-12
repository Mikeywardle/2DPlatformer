#pragma once

#include <ecs/System.h>

class BattleCameraSystem : public System
{
public:

	BattleCameraSystem(World* world);

	virtual void OnInput(const float deltaTime, const InputData* inputData) override;
};