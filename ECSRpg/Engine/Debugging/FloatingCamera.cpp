#include "FloatingCamera.h"

#include <Game.h>
#include <Core/World.h>
#include <Core/Transform.h>
#include <Maths/Vector2.h>
#include <Inputs/InputReceiver.h>
#include <Core/GameTime.h>

void FloatingCameraMovementSystem::OnFrame(float deltaTime)
{
	GLFWwindow* gameWindow = GameWindow;

	bool wPressed = glfwGetKey(gameWindow, GLFW_KEY_W) == GLFW_PRESS;
	bool aPressed = glfwGetKey(gameWindow, GLFW_KEY_A) == GLFW_PRESS;
	bool sPressed = glfwGetKey(gameWindow, GLFW_KEY_S) == GLFW_PRESS;
	bool dPressed = glfwGetKey(gameWindow, GLFW_KEY_D) == GLFW_PRESS;

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
	inputReceiver = new InputReceiver();
	inputReceiver->AddMousePositionCallback(this, &FloatingCameraMovementSystem::OnMousePositionChanged);
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

		rotation.z += -Delta.y * fcc->LookSpeed * deltaTime;
		rotation.y += Delta.x * fcc->LookSpeed * deltaTime;

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
