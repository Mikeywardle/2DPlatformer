#pragma once

#include <ecs/System.h>

struct FloatingCameraComponent
{
	FloatingCameraComponent() = default;
	FloatingCameraComponent(float MovementSpeed,float LookSpeed);

	float MovementSpeed;
	float LookSpeed;
};

typedef int InputKey;
typedef int  InputType;

struct Vector2;

class InputReceiver;

class FloatingCameraMovementSystem : public System
{

public:
	FloatingCameraMovementSystem(World* world);

	virtual void OnFrame(float deltaTime) override;

private:
	InputReceiver* inputReceiver;

	void OnMousePositionChanged(Vector2 Position, Vector2 Delta);
	void BindEvent();

	void OnRightClickPress(InputKey key, InputType type);
	void OnRightClickRelease(InputKey key, InputType type);
};
