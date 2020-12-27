#pragma once

#include <queue>
#include "Entity.h"

class EntitiesManager
{
public:

	Entity CreateEntity();
	void DestroyEntity(Entity entity);

private:

	std::queue<Entity> availableEntities;

	unsigned int aliveEntities = 0;
};