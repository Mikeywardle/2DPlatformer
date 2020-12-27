#pragma once
#include "EcsContext.h"
#include <vector>

class World;

class System
{
public:

	System(World* world);

	virtual void OnFrame(float deltaTime) = 0;

protected:

	World* world;
};

struct OptionalSystemsConfig
{
	std::vector<System*> gameSystems;
};
