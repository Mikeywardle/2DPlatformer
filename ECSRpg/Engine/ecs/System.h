#pragma once

class World;
struct InputData;

class System
{
public:

	System(World* world);
	virtual ~System() = default;

	virtual void OnInput(const float deltaTime, const InputData* inputData) {};
	virtual void OnFrame(const float deltaTime) {};

protected:

	World* world;
};
