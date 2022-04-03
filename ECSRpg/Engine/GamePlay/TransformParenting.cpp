#include "TransformParenting.h"
#include <Core/World.h>
#include <Core/SceneTransformComponents.h>

PositionAttatchmentComponent::PositionAttatchmentComponent(Entity Parent, Vector3 LocalOffset, bool DestroyOnNoParent)
{
	this->DestroyOnNoParent = DestroyOnNoParent;
	this->Parent = Parent;
	this->LocalOffset = LocalOffset;
}

void PositionAttachmentSystem::OnFrame(float deltaTime)
{
	ForEntities(world, PositionAttatchmentComponent, SceneTransformComponent)
	{
		PositionAttatchmentComponent* pac = world->GetComponent<PositionAttatchmentComponent>(entity);
		SceneTransformComponent* parentTransform = world->GetComponent<SceneTransformComponent>(pac->Parent);

		if (parentTransform != nullptr)
		{
			SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(entity);

			transform->SetPosition(parentTransform->TransformPoint(pac->LocalOffset));
			transform->SetRotation(parentTransform->GetRotation());
		}
		else if (pac->DestroyOnNoParent)
		{
			world->DestroyEntity(entity);
		}

	}
}
