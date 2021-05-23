#include "FloatingCamera.h"

#include <Game.h>
#include <Core/World.h>
#include <Core/Transform.h>
#include <Maths/Vector2.h>
#include <Inputs/InputReceiver.h>
#include <Core/GameTime.h>

FloatingCameraMovementSystem::FloatingCameraMovementSystem(World* world)
	:System(world)
{
	inputReceiver = new InputReceiver();
	inputReceiver->AddButtonBinding(this, &FloatingCameraMovementSystem::OnRightClickPress, "Move", InputTypes::BUTTON_PRESSED);
	inputReceiver->AddButtonBinding(this, &FloatingCameraMovementSystem::OnRightClickRelease, "Move", InputTypes::BUTTON_RELEASED);
}

void FloatingCameraMovementSystem::OnFrame(float deltaTime)
{
	bool wPressed = inputReceiver->KeyPressed(InputKeys::KEYBOARD_W_KEY);
	bool aPressed = inputReceiver->KeyPressed(InputKeys::KEYBOARD_A_KEY);
	bool sPressed = inputReceiver->KeyPressed(InputKeys::KEYBOARD_S_KEY);
	bool dPressed = inputReceiver->KeyPressed(InputKeys::KEYBOARD_D_KEY);

	float forwardMove = (wPressed + (-1 * sPressed))* deltaTime;
	float rightMove = (dPressed + (-1 * aPressed)) * deltaTime;

	std::vector<Entity> entities = world->GetEntities<FloatingCameraComponent, Transform>();

	for (Entity entity : entities)
	{
		Transform* t = world->GetComponent<Transform>(entity);
		FloatingCameraComponent* fcc = world->GetComponent<FloatingCameraComponent>(entity);

		t->AddTranslation(t->GetForward() * forwardMove * fcc->MovementSpeed);
		t->AddTranslation(t->GetRight() * rightMove * fcc->MovementSpeed);
	}
}

void FloatingCameraMovementSystem::BindEvent()
{
	inputReceiver->AddMousePositionCallback(this, &FloatingCameraMovementSystem::OnMousePositionChanged);
}

void FloatingCameraMovementSystem::OnRightClickPress(InputKey key, InputType type)
{
	inputReceiver->AddMousePositionCallback(this, &FloatingCameraMovementSystem::OnMousePositionChanged);
}

void FloatingCameraMovementSystem::OnRightClickRelease(InputKey key, InputType type)
{
	inputReceiver->RemoveMousePositionBinding();
}


void FloatingCameraMovementSystem::OnMousePositionChanged(Vector2 Position, Vector2 Delta)
{
	std::vector<Entity> entities = world->GetEntities<FloatingCameraComponent, Transform>();
	float deltaTime = DELTA_TIME;

	for (Entity entity : entities)
	{
		Transform* t = world->GetComponent<Transform>(entity);
		FloatingCameraComponent* fcc = world->GetComponent<FloatingCameraComponent>(entity);

		Vector3 rotation = t->GetRotation();

		rotation.z += -Delta.y * fcc->LookSpeed;// *deltaTime;
		rotation.y += Delta.x * fcc->LookSpeed;// *deltaTime;

		if (rotation.z > 89.0f)
			rotation.z = 89.0f;
		if (rotation.z < -89.0f)
			rotation.z = -89.0f;

		t->SetRotation(rotation);
	}
}

FloatingCameraComponent::FloatingCameraComponent(float MovementSpeed, float LookSpeed)
{
	this->MovementSpeed = MovementSpeed;
	this->LookSpeed = LookSpeed;
}
