#include "CameraRotationSystem.h"

#include <Core/World.h>
#include <Maths/Vector2.h>

#include <Inputs/InputReceiver.h>

#include <Components/PlayerTags.h>
#include <Components/CameraRotationComponent.h>

#include <Maths/Transform.h>



CameraRotationSystem::CameraRotationSystem(World* world)
	:System(world)
{
	inputReceiver = new InputReceiver();

	inputReceiver->AddMousePositionCallback(this, &CameraRotationSystem::OnMousePositionChanged);
}

void CameraRotationSystem::OnFrame(float deltaTime){}

void CameraRotationSystem::OnMousePositionChanged(Vector2 Position, Vector2 Delta)
{
	std::vector<Entity> entities = world->GetEntities<CurrentPossesedPlayer, CameraRotationComponent, Transform>();

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		CameraRotationComponent* rotator = world->GetComponent<CameraRotationComponent>(entity);

		const Vector3 currentRotation = transform->GetRotation();

		Vector3 NewRotation = currentRotation;

		NewRotation.y += Delta.x * rotator->RotationSpeed;
		NewRotation.z += -Delta.y * rotator->RotationSpeed;

		if (NewRotation.z > 89.0f)
			NewRotation.z = 89.0f;
		if (NewRotation.z < -89.0f)
			NewRotation.z = -89.0f;

		transform->SetRotation(NewRotation);

	}
}
