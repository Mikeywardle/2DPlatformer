#include "BuildingMarkerSystem.h"

#include <Core/World.h>
#include <Core/Transform.h>
#include <Camera/CameraComponent.h>

#include "../Components/BuildingPlacementMarker.h" 
#include <math.h>

void BuildingMarkerSystem::OnMousePositionChanged(Vector2 Position, Vector2 Delta)
{
	const Entity CameraEntity = CameraComponent::GetMainCamera();

	CameraComponent* MainCamera = world->GetComponent <CameraComponent>(CameraEntity);
	Transform* CameraTransform = world->GetComponent <Transform>(CameraEntity);

	std::vector<Entity> entities = world->GetEntities<BuildingPlacementMarker, Transform>();
	Vector3 MousePosition = MainCamera->ScreenPositionToWorldPosition(Position.x, Position.y , abs(CameraTransform->GetPosition().z), CameraTransform);

	MousePosition.x = (int)MousePosition.x;
	MousePosition.y = (int)MousePosition.y;

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		transform->SetPosition(MousePosition);
	}
}
