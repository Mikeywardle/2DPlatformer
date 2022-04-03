#include "SceneTransformSystem.h"

#include <Core/World.h>
#include <Core/SceneTransformComponents.h>

SceneTransformSystem::SceneTransformSystem(World* world)
{
	this->world = world;
}

void SceneTransformSystem::Update(const float& deltaTime)
{

	std::vector<SceneTransformComponent>* transforms = world->GetComponents<SceneTransformComponent>();

	if (transforms == nullptr)
		return;

	std::vector<Entity> destroyedEntities;

	for (SceneTransformComponent& transform : *transforms)
	{
		if (transform.IsToBeDestroyed())
		{
			destroyedEntities.push_back(world->GetComponentOwner<SceneTransformComponent>(&transform));
		}
	}

	for (Entity entity : destroyedEntities)
	{
		world->DestroyEntity(entity);
	}
}
