#pragma once
#include <Inputs/InputValues.h>
#include <ecs/System.h>
#include <Core/World.h>
#include <Physics/Collisions/CollisionEvent.h>

class PlayerMovementSystem : public System
{
public:
	PlayerMovementSystem(World* world) : System(world)
	{
		BindEvents();
		world->AddListener<CollisionEvent>(this, &PlayerMovementSystem::OnPlayerLand);
	}

	virtual void OnFrame(float deltaTime) override;

private:

	class InputReceiver* inputReceiver;

	void OnJump(InputKey key, InputType type);

	void OnPlayerLand(CollisionEvent collisions);

	void OnLeftPressed(InputKey key, InputType type);
	void OnLeftReleased(InputKey key, InputType type);

	void OnRightPressed(InputKey key, InputType type);
	void OnRightReleased(InputKey key, InputType type);

	void BindEvents();
};
