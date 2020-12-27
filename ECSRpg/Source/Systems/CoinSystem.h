#pragma once

#include <ecs/System.h>

class CoinSystem : public System
{
public:
	CoinSystem(World* world) : System(world)
	{

	}

	virtual void OnFrame(float deltaTime) override;
};