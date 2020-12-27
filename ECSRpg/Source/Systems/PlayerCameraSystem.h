#pragma once

#include <ecs/System.h>

class PlayerCameraSystem : public System
{
public:
	PlayerCameraSystem(World* world) : System(world) {};

	virtual void OnFrame(float deltaTime) override;
};
