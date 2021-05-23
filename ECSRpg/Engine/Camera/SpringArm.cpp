#include "SpringArm.h"

#include <Core/World.h>
#include <Core/Transform.h>
#include <Physics/PhysicsSystem.h>

SpringArmComponent::SpringArmComponent(float InDistance, Entity InChild, Entity InParent)
{
	distance = InDistance;
	child = InChild;
	parent = InParent;
}


SpringArmSystem::SpringArmSystem(World* world)
	:System(world)
{
}

void SpringArmSystem::OnFrame(float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<SpringArmComponent, Transform>();

	PhysicsSystem* physicsSystem = world->GetPhysicsSystem();

	for (Entity arm : entities)
	{
		SpringArmComponent* springArm = world->GetComponent<SpringArmComponent>(arm);

		Transform* childTransform = world->GetComponent<Transform>(springArm->child);
		Transform* parentTransform = world->GetComponent<Transform>(springArm->parent);

		if (childTransform == nullptr || parentTransform == nullptr)
			continue;

		Transform* armTransform = world->GetComponent<Transform>(arm);

		armTransform->SetPosition(parentTransform->GetPosition());

		const Vector3 springArmForward = armTransform->GetForward();
		const Vector3 springArmPos = armTransform->GetPosition();

		const float NegativeDistance = -1.0f * springArm->distance;

		const Vector3 springArmEnd = springArmPos + (springArmForward * NegativeDistance);

		const RaycastingResult result =  physicsSystem->CastRay(springArmPos, springArmEnd, springArm->parent);

		Vector3 CameraOffset;

		if (result.hasHit)
		{
			CameraOffset = springArmForward * NegativeDistance * result.Distance;
		}
		else
		{
			CameraOffset = springArmForward * NegativeDistance;
		}

		childTransform->SetPosition(springArmPos);
		childTransform->SetRotation(armTransform->GetRotation());
		childTransform->AddTranslation(CameraOffset);
	}
}