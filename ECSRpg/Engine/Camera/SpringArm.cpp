#include "SpringArm.h"

#include <Core/World.h>
#include <Core/SceneTransformComponents.h>
#include <Physics/PhysicsSystem.h>

SpringArmComponent::SpringArmComponent(float InDistance, float InLerpSpeed, Entity InChild, Entity InParent)
{
	distance = InDistance;
	lerpSpeed = InLerpSpeed;
	child = InChild;
	parent = InParent;
}


SpringArmSystem::SpringArmSystem(World* world)
	:System(world)
{
}

void SpringArmSystem::OnFrame(float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<SpringArmComponent, SceneTransformComponent>();

	PhysicsSystem* physicsSystem = world->GetPhysicsSystem();

	for (Entity arm : entities)
	{
		SpringArmComponent* springArm = world->GetComponent<SpringArmComponent>(arm);

		SceneTransformComponent* childTransform = world->GetComponent<SceneTransformComponent>(springArm->child);
		SceneTransformComponent* parentTransform = world->GetComponent<SceneTransformComponent>(springArm->parent);

		if (childTransform == nullptr || parentTransform == nullptr)
			continue;

		SceneTransformComponent* armTransform = world->GetComponent<SceneTransformComponent>(arm);

		armTransform->SetPosition(parentTransform->GetPosition());

		const Vector3 springArmForward = armTransform->GetForward();
		const Vector3 springArmPos = armTransform->GetPosition();

		const float NegativeDistance = -1.0f * springArm->distance;

		const Vector3 springArmEnd = springArmPos + (springArmForward * NegativeDistance);

		const Ray ray = Ray(springArmPos, springArmEnd);
		const RaycastingResult result =  physicsSystem->CastRay(ray, springArm->parent);

		Vector3 CameraOffset;

		if (result.hasHit)
		{
			CameraOffset = springArmForward * NegativeDistance * result.Distance;
		}
		else
		{
			CameraOffset = springArmForward * NegativeDistance;
		}

		childTransform->SetRotation(armTransform->GetRotation());

		const Vector3 oldCameraPosition = childTransform->GetPosition();

		childTransform->SetPosition(Vector3::Lerp(oldCameraPosition
			, springArmPos+CameraOffset
			, springArm->lerpSpeed)
		);
	}
}