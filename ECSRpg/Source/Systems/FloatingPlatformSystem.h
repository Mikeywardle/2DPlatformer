#pragma once

#include <ecs/System.h>

class FloatingPlatformSystem : public System
{
public:
	FloatingPlatformSystem(World* world) : System(world) {}

	virtual void OnFrame(float deltaTime) override;
};
