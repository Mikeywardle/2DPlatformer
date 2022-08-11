#pragma once

#include <ecs/System.h>

#include <Maths/MathsTypes.h>

class CollisionEvent;

class PlayerMovementSystem : public System
{
public:

	PlayerMovementSystem(World* world);
	~PlayerMovementSystem();

	virtual void OnFrame(float deltaTime) override;
	virtual void OnInput(const float deltaTime, const InputData* inputData) override;

private:

	void CheckPlayersOnGround();
	void ProcessMovementInputs(const InputData* inputData);
	void ProcessJumps();
	void OnPlayerCollided(CollisionEvent& collisionEvent);
	
	void Jump();

};