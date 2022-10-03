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
	world->ForEntities<PositionAttatchmentComponent, SceneTransformComponent>
		(
			[&](const Entity entity, PositionAttatchmentComponent* pac, SceneTransformComponent* transform)
			{		
				SceneTransformComponent* parentTransform = world->GetComponent<SceneTransformComponent>(pac->Parent);

				if (parentTransform != nullptr)
				{
					transform->SetPosition(parentTransform->TransformPoint(pac->LocalOffset));
					transform->SetRotation(parentTransform->GetRotation());
				}
				else if (pac->DestroyOnNoParent)
				{
					world->DestroyEntity(entity);
				}

			}
		);
}
