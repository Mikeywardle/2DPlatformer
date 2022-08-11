#include "GravityPanelSystem.h"

#include <Core/World.h>
#include <Physics/Collisions/CollisionEvent.h>

#include <Components/GravityPanelComponent.h>
#include <Components/PlayerMovementComponent.h>

#include <Core/SceneTransformComponents.h>
#include <Physics/Forces/Gravity.h>

GravityPanelSystem::GravityPanelSystem(World* world)
	:System(world)
{
	world->AddListener<TriggerOverlapBeginEvent&>(this, &GravityPanelSystem::OnTriggerOverlap);
}

GravityPanelSystem::~GravityPanelSystem()
{

}

void GravityPanelSystem::OnTriggerOverlap(TriggerOverlapBeginEvent& event)
{
	for (const PhysicsCollisionResult& result : event.results)
	{
		const Entity entityA = result.entityA;
		const Entity entityB = result.entityB;

		Entity panelEntity = NO_ENTITY;
		Entity playerEntity = NO_ENTITY;

		if (world->HasComponent<GravityPanelComponent>(entityA) && world->HasComponent<PlayerMovementComponent>(entityB))
		{
			panelEntity = entityA;
			playerEntity = entityB;

		}
		else if (world->HasComponent<GravityPanelComponent>(entityB) && world->HasComponent<PlayerMovementComponent>(entityA))
		{
			panelEntity = entityB;
			playerEntity = entityA;
		}

		if (panelEntity != NO_ENTITY)
		{
			printf("Overlap!!!");
			const SceneTransformComponent* panelTransform = world->GetComponent<SceneTransformComponent>(panelEntity);
			const Vector3 panelUp = panelTransform->GetUp();

			GravityComponent* playerGravity = world->GetComponent<GravityComponent>(playerEntity);
			SceneTransformComponent* playerTransform = world->GetComponent<SceneTransformComponent>(playerEntity);

			playerGravity->GravityScale = panelUp;
			Vector3 PlayerRotation = playerTransform->GetRotation();
			PlayerRotation.x += 180;

			playerTransform->SetRotation(PlayerRotation);
		}

	}
}
