#include "TransformParenting.h"
#include <Core/World.h>
#include <Maths/Transform.h>

PositionAttatchmentComponent::PositionAttatchmentComponent(Entity Parent, Vector3 LocalOffset, bool DestroyOnNoParent)
{
	this->DestroyOnNoParent = DestroyOnNoParent;
	this->Parent = Parent;
	this->LocalOffset = LocalOffset;
}

void PositionAttachmentSystem::OnFrame(float deltaTime)
{
	ForEntities(world, PositionAttatchmentComponent, Transform)
	{
		PositionAttatchmentComponent* pac = world->GetComponent<PositionAttatchmentComponent>(entity);
		Transform* parentTransform = world->GetComponent<Transform>(pac->Parent);

		if (parentTransform != nullptr)
		{
			Transform* transform = world->GetComponent<Transform>(entity);

			transform->SetPosition(parentTransform->TransformPoint(pac->LocalOffset));
			transform->SetRotation(parentTransform->GetRotation());
		}
		else if (pac->DestroyOnNoParent)
		{
			world->DestroyEntity(entity);
		}

	}
}
