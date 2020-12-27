#include "EntitiesManager.h"

Entity EntitiesManager::CreateEntity()
{
	Entity newEntity;

	if (availableEntities.size() > 0)
	{
		newEntity = availableEntities.front();
		availableEntities.pop();
	}
	else
	{
		newEntity = aliveEntities;
	}

	++aliveEntities;
	return newEntity;
}

void EntitiesManager::DestroyEntity(Entity entity)
{
	availableEntities.push(entity);
	--aliveEntities;
}
