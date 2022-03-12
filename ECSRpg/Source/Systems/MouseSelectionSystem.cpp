#include "MouseSelectionSystem.h"

#include <Inputs/InputData.h>

#include <Camera/CameraComponent.h>

#include <Core/World.h>

#include <Physics/PhysicsSystem.h>

#include <Components/UnitComponents.h>

MouseSelectionSystem::MouseSelectionSystem(World* world)
	: System(world)
{}

void MouseSelectionSystem::OnInput(const float deltaTime, const InputData* inputData)
{
	Vector2 MousePostion = inputData->MousePosition;

	Entity cameraEntity = CameraComponent::GetMainCamera();

	CameraComponent* cameraComponent = world->GetComponent<CameraComponent>(cameraEntity);
	Transform* transform = world->GetComponent<Transform>(cameraEntity);

	const Vector3 MouseRayDirection = cameraComponent->ScreenSpaceLocationToWorldSpaceDirection(MousePostion, transform, world->GetGameWindow());

	PhysicsSystem* physicsSystem = world->GetPhysicsSystem();

	const Vector3 RayStart = transform->GetPosition();

	const Ray ray = Ray(RayStart, RayStart + (MouseRayDirection * 1000.f));
	RaycastingResult result = physicsSystem->CastRay(ray, cameraEntity);

	Entity marker = world->GetEntity<MouseSelectionDebugMarker, Transform>();
	Transform* markerTransform = world->GetComponent<Transform>(marker);

	if (result.hasHit)
	{
		markerTransform->SetPosition(result.Position);

		const bool selectable = world->HasComponent<SelectableUnitComponent>(result.HitEntity);
	}
	else
	{
		markerTransform->SetPosition(Vector3(1000,1000,1000));
	}


}
