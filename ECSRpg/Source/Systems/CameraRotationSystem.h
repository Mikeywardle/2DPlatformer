#pragma once

#include <ecs/System.h>

class InputReceiver;
struct Vector2;

class CameraRotationSystem : public System
{
public:
	CameraRotationSystem(World* world);


	virtual void OnInput(const float deltaTime, const InputData* inputData) override;

private:
};
