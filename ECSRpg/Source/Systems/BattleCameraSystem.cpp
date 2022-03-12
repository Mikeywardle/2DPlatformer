#include "BattleCameraSystem.h"


#include "Config/TestInputConfig.h"

#include <Inputs/InputData.h>

#include <Core/World.h>

#include <Components/BattleCameraComponent.h>

#include <Camera/CameraComponent.h>

BattleCameraSystem::BattleCameraSystem(World* world)
	:System(world)
{
}

void BattleCameraSystem::OnInput(const float deltaTime, const InputData* inputData)
{
	bool wPressed = inputData->GetInputValue(TestConfigInputId::Up, InputTypes::BUTTON_IS_DOWN);
	bool sPressed = inputData->GetInputValue(TestConfigInputId::Down, InputTypes::BUTTON_IS_DOWN);

	bool aPressed = inputData->GetInputValue(TestConfigInputId::Left, InputTypes::BUTTON_IS_DOWN);
	bool dPressed = inputData->GetInputValue(TestConfigInputId::Right, InputTypes::BUTTON_IS_DOWN);

	bool qPressed = inputData->GetInputValue(TestConfigInputId::Turn_Left, InputTypes::BUTTON_IS_DOWN);
	bool ePressed = inputData->GetInputValue(TestConfigInputId::Turn_Right, InputTypes::BUTTON_IS_DOWN);

	bool lPressed = inputData->GetInputValue(TestConfigInputId::Left_click, InputTypes::BUTTON_IS_DOWN);
	bool rPressed = inputData->GetInputValue(TestConfigInputId::Right_click, InputTypes::BUTTON_IS_DOWN);

	ForEntities(world, BattleCameraComponent, CameraComponent, Transform )
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		BattleCameraComponent* camera = world->GetComponent<BattleCameraComponent>(entity);
		
		transform->AddRotation(Vector3(0,(ePressed - qPressed)*2.0f, (lPressed - rPressed) * 2.0f));

		Vector3 CameraMoveForwards = transform->GetForward() * (wPressed - sPressed);
		//CameraMoveForwards.y = 0;
		CameraMoveForwards = Vector3::Normalize(CameraMoveForwards);

		const Vector3 CameraMoveRight = transform->GetRight() * (dPressed - aPressed);

		const Vector3 CameraMove = Vector3::Normalize(CameraMoveForwards + CameraMoveRight);

		transform->AddTranslation(CameraMove * camera->MovementSpeed * deltaTime);





	}
}
