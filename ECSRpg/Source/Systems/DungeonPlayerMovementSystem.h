#pragma once

#include <ecs/System.h>

class DungeonPlayerMovementSystem final : public System
{
public:
	DungeonPlayerMovementSystem(World* world);

	virtual void OnFrame(float deltaTime) override;
	virtual void OnInput(const float deltaTime, const InputData* inputData) override;
};