#pragma once

#include <ecs/System.h>

class FloatingPlatformSystem : public System
{
public:
	FloatingPlatformSystem(World* world);

	virtual void OnFrame(const float deltaTime) override;
};
