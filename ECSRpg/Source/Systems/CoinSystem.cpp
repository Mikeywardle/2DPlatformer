#include "CoinSystem.h"

#include <Core/World.h>
#include <Physics/PhysicsSystem.h>

#include "../Components/CoinPickupComponent.h"
#include "../Components/PlayerMovementComponent.h"

void CoinSystem::OnFrame(float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<CoinPickup, Transform>();

	PhysicsSystem* ps = world->GetPhysicsSystem();
	std::vector<Entity> collectedCoins;

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);

		std::vector<Entity> hitPlayers = ps->CastSphere<PlayerMovementComponent>(transform->GetPosition(), 1.f);

		if (hitPlayers.size() != 0)
			collectedCoins.push_back(entity);
	}

	for (Entity coin : collectedCoins)
	{
		world->DestroyEntity(coin);
	}
}
