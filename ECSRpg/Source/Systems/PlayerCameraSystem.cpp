#include "PlayerCameraSystem.h"

#include <Core/World.h>
#include <Camera/CameraComponent.h>
#include "../Components/PlayerMovementComponent.h"

void PlayerCameraSystem::OnFrame(float deltaTime)
{
	Entity MainCamera = CameraComponent::GetMainCamera();

	Transform* CameraTransform = world->GetComponent<Transform>(MainCamera);
	Vector3 CameraPosition = CameraTransform->GetPosition();

	Entity Player = world->GetEntities<PlayerMovementComponent>()[0];

	Vector3 PlayerPosition= world->GetComponent<Transform>(Player)->GetPosition();


	if (PlayerPosition.x > CameraPosition.x + 5)
	{
		CameraPosition.x = PlayerPosition.x - 5;
	} 
	else if(PlayerPosition.x < CameraPosition.x - 5)
	{
		CameraPosition.x = PlayerPosition.x + 5;
	}

	if (PlayerPosition.y > CameraPosition.y + 5)
	{
		CameraPosition.y = PlayerPosition.y - 5;
	}
	else if (PlayerPosition.y < CameraPosition.y - 5)
	{
		CameraPosition.y = PlayerPosition.y + 5;
	}

	CameraTransform->SetPosition(CameraPosition);



}
