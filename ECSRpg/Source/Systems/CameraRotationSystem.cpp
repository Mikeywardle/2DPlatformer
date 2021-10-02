#include "CameraRotationSystem.h"

#include <Core/World.h>
#include <Maths/Vector2.h>

#include <Components/PlayerTags.h>
#include <Components/CameraRotationComponent.h>

#include <Maths/Transform.h>

#include <Inputs/InputData.h>



CameraRotationSystem::CameraRotationSystem(World* world)
	:System(world)
{}

void CameraRotationSystem::OnInput(const float deltaTime, const InputData* inputData)
{
	const Vector2 Delta = inputData->MouseDelta;

	std::vector<Entity> entities = world->GetEntities<CurrentPossesedPlayer, CameraRotationComponent, Transform>();

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		CameraRotationComponent* rotator = world->GetComponent<CameraRotationComponent>(entity);

		const Vector3 currentRotation = transform->GetRotation();

		Vector3 NewRotation = currentRotation;

		NewRotation.y += Delta.x * rotator->RotationSpeed * deltaTime;
		NewRotation.z += -Delta.y * rotator->RotationSpeed * deltaTime;

		if (NewRotation.z > 89.0f)
			NewRotation.z = 89.0f;
		if (NewRotation.z < -89.0f)
			NewRotation.z = -89.0f;

		transform->SetRotation(NewRotation);
	}
}
