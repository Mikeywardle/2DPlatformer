#pragma once

#include <ecs/System.h>

struct MouseSelectionDebugMarker {};

class MouseSelectionSystem : public System
{
public:
	MouseSelectionSystem(World* world);

	virtual void OnInput(const float deltaTime, const InputData* inputData) override;
};
