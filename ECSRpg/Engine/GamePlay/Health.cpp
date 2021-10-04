#include "Health.h"

#include <Core/World.h>

HealthComponent::HealthComponent(short Health, bool canBeInvincible, float InvincibilityTime)
{
	this->Health = Health;
	this->canBeInvincible = canBeInvincible;
	this->InvincibilityTime = InvincibilityTime;
}

void HealthComponent::DealDamage(short Damage)
{
	Health -= Damage * !isInvincible;

	//Trigger invincibilty for player
	if (canBeInvincible && !isInvincible)
	{
		isInvincible = true;
		remainingInvincibilityTime = InvincibilityTime;
	}

}

void InvincibilityTimingSystem::OnFrame(float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<HealthComponent>();

	for (Entity entity : entities)
	{
		HealthComponent* health = world->GetComponent<HealthComponent>(entity);

		if (!health->isInvincible)
			continue;

		health->remainingInvincibilityTime -= deltaTime;

		if (health->remainingInvincibilityTime <= 0)
			health->isInvincible = false;
	}
}

void NoHealthCleanupSystem::OnFrame(float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<HealthComponent>();

	for (Entity entity : entities)
	{
		HealthComponent* health = world->GetComponent<HealthComponent>(entity);

		if (health->Health <= 0)
			world->DestroyEntity(entity);
	}
}
