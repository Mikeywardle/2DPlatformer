#include "FloatingPlatformSystem.h"
#include "../Components/FloatingPlatformComponent.h"
#include <Physics/RigidBody.h>
#include <Core/World.h>

void FloatingPlatformSystem::OnFrame(float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<FloatingPlatformComponent, RigidBodyComponent, Transform>();

	for (Entity entity : entities)
	{
		Transform* t = world->GetComponent<Transform>(entity);
		FloatingPlatformComponent* platform = world->GetComponent<FloatingPlatformComponent>(entity);
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);

		Vector3 platformPosition = t->GetPosition();
		const Vector3 pathVector = platform->end - platform->start;
		const Vector3 platformVector = platformPosition - platform->start;

		const float vectorsDot = Vector3::DotProduct(pathVector, platformVector);

		float platformPositionT = Vector3::Magnitude(platformVector)/Vector3::Magnitude(pathVector);

		if (vectorsDot < 0)
			platformPositionT *= -1;


		platform->movingForwards = platform->movingForwards && platformPositionT < 1 || platformPositionT < 0;

		Vector3 movingDirection = Vector3::Normalize(platform->end - platform->start);

		if (!platform->movingForwards)
			movingDirection *= -1;

		float platformSpeed = platform->speed;
		rb->velocity = movingDirection * platformSpeed;

	}
}
