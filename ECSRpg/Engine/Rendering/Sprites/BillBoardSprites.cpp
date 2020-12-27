#include "BillBoardSprites.h"
#include <Core/Transform.h>
#include <Camera/CameraComponent.h>
#include <math.h>
#include <Maths/Trigonometry.h>
#include <Core/World.h>

void BillBoardSpriteSystem::OnFrame(float deltaTime)
{
	
	std::vector<Entity>entities = world->GetEntities<BillBoardComponent,Transform>();

	Entity cameraEntity = CameraComponent::GetMainCamera();
	Transform* cameraTranform = world->GetComponent<Transform>(cameraEntity);
	Vector3 cameraposition = cameraTranform->GetPosition();

	Vector3 billboardPosition;
	Vector3 diff;
	Vector3 rotation;

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		billboardPosition = transform->GetPosition();
		rotation = transform->GetRotation();

		BillBoardComponent* billboard = world->GetComponent<BillBoardComponent>(entity);


		diff = cameraposition - billboardPosition;

		//if (billboard->rotateX)
		//{
		//	rotation.x = RadiansToDegrees(atan(diff.z / diff.y));
		//}

		if (billboard->rotateY)
		{
			rotation.y = RadiansToDegrees(atan(diff.x / diff.z));
		}

		//if (billboard->rotateZ)
		//{
		//	rotation.z = RadiansToDegrees(atan(diff.y/ diff.x));
		//}

		transform->SetRotation(rotation);
	}
}
