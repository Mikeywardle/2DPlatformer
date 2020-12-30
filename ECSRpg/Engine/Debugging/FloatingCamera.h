#pragma once

#include <ecs/System.h>

struct FloatingCameraComponent
{
	FloatingCameraComponent() = default;
	FloatingCameraComponent(float MovementSpeed,float LookSpeed);

	float MovementSpeed;
	float LookSpeed;
};

struct Vector2;

class FloatingCameraMovementSystem : public System
{

public:
	FloatingCameraMovementSystem(World* world) : System(world) 
	{
		BindEvent();
	};

	virtual void OnFrame(float deltaTime) override;

private:
	class InputReceiver* inputReceiver;

	void OnMousePositionChanged(Vector2 Position, Vector2 Delta);
	void BindEvent();
};
