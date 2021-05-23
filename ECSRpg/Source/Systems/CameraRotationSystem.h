#pragma once

#include <ecs/System.h>

class InputReceiver;
struct Vector2;

class CameraRotationSystem : public System
{
public:
	CameraRotationSystem(World* world);

	virtual void OnFrame(float deltaTime) override;

private:
	InputReceiver* inputReceiver;

	void OnMousePositionChanged(Vector2 Position, Vector2 Delta);
};
