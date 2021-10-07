#pragma once

#include <queue>
#include "Entity.h"

class EntitiesManager
{
public:

	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	void ResetState();

	unsigned int GetAliveEntities() const;

private:

	std::queue<Entity> availableEntities;

	unsigned int aliveEntities = 0;
};