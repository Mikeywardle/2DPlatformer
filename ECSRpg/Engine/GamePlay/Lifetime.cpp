#include "Lifetime.h"
#include <Core/World.h>


LifetimeComponent::LifetimeComponent(float lifetime, Entity entity, bool doesDecay)
{
	lifeRemaining = lifetime;
	this->entity = entity;
	this->doesDecay = doesDecay;
}

void LifeTimeDecaySystem::OnFrame(float deltaTime)
{
	std::vector<LifetimeComponent>* lifetimes = world->GetComponents<LifetimeComponent>();

	if (lifetimes == nullptr)
		return;

	std::vector<Entity> destroyedEntities;

	for (LifetimeComponent& lifetime : *lifetimes)
	{
		lifetime.lifeRemaining -= deltaTime * lifetime.doesDecay;

		if (lifetime.lifeRemaining <= 0)
			destroyedEntities.push_back(lifetime.entity);

	}

	for (Entity entity : destroyedEntities)
	{
		world->DestroyEntity(entity);
	}

}


