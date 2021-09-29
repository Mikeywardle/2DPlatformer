#include "BillBoardSprites.h"
#include <Maths/Transform.h>
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

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		Vector3 billboardPosition = transform->GetPosition();
		Vector3 rotation = transform->GetRotation();

		BillBoardComponent* billboard = world->GetComponent<BillBoardComponent>(entity);

		Vector3 diff = cameraposition - billboardPosition;


		//Y-Axis Rotation
		{
			float diffFrac = diff.x / diff.z;

			float newRotation = RadiansToDegrees(atan(diffFrac));

			if (diff.z > 0)
				newRotation -= 180;

			rotation.y = newRotation;
		}

		transform->SetRotation(rotation);
	}
}
