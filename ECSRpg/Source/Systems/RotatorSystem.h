#pragma once

#include <ecs/System.h>
#include <Core/World.h>
#include <Core/Transform.h>
#include "../Components/RotatorComponent.h"

class RotatorSystem : public System
{
public:
	RotatorSystem(World* world) : System(world)	{}

	virtual void OnFrame(float deltaTime)
	{
		std::vector<Entity> entities = world->GetEntities<RotatorComponent,Transform>();

		for (Entity entity : entities)
		{
			Transform* transform = world->GetComponent<Transform>(entity);
			RotatorComponent* rotator = world->GetComponent<RotatorComponent>(entity);

			transform->AddRotation(rotator->deltaPerSecond*deltaTime);
		}
	}
};
