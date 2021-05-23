#pragma once

#include <ecs/System.h>

class InputReceiver;

typedef int InputKey;
typedef int InputType;


class PlayerMovementSystem : public System
{

public:

	PlayerMovementSystem(World* world);
	~PlayerMovementSystem();

	virtual void OnFrame(float deltaTime);

private:

	void CheckPlayersOnGround();
	void ProcessMovementInputs();
	
	void Jump(InputKey key, InputType type);

	InputReceiver* inputReceiver;

};