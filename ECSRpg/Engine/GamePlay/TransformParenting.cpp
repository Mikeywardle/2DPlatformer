#include "TransformParenting.h"
#include <Core/World.h>
#include <Core/Transform.h>

PositionAttatchmentComponent::PositionAttatchmentComponent(Entity Parent, Vector3 LocalOffset, bool DestroyOnNoParent)
{
	this->DestroyOnNoParent = DestroyOnNoParent;
	this->Parent = Parent;
	this->LocalOffset = LocalOffset;
}

void PositionAttachmentSystem::OnFrame(float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<PositionAttatchmentComponent, Transform>();

	for (Entity entity : entities)
	{
		PositionAttatchmentComponent* pac = world->GetComponent<PositionAttatchmentComponent>(entity);
		Transform* parentTransform = world->GetComponent<Transform>(pac->Parent);

		if (parentTransform != nullptr)
		{
			Transform* transform = world->GetComponent<Transform>(entity);

			transform->SetPosition(parentTransform->GetPosition() + pac->LocalOffset);
		}
		else if (pac->DestroyOnNoParent)
		{
			world->DestroyEntity(entity);
		}

	}
}
