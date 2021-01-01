#include "PlayerMovementSystem.h"
#include <Inputs/InputReceiver.h>

#include "../Components/PlayerMovementComponent.h"
#include <Physics/PhysicsSystem.h>
#include <Physics/Collisions/CollisionDetection.h>

#include <Physics/RigidBody.h>

#include "SubSystems/InAirStateSubSystem.h"

void PlayerMovementSystem::OnFrame(float deltaTime)
{
	CheckInAirState(world);

	std::vector<Entity> entities = world->GetEntities<PlayerMovementComponent, RigidBodyComponent>();

	bool forwardsPressed = inputReceiver->BindingPressed("Forwards");
	bool backwardsPressed = inputReceiver->BindingPressed("Backwards");

	bool leftPressed = inputReceiver->BindingPressed("Left");
	bool rightPressed = inputReceiver->BindingPressed("Right");

	for (Entity entity : entities)
	{
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);
		PlayerMovementComponent* p = world->GetComponent<PlayerMovementComponent>(entity);

		const Vector3 MoveDir = (VECTOR3_RIGHT * (rightPressed - leftPressed)) +(VECTOR3_FORWARD * (forwardsPressed - backwardsPressed));
		float force;

		if (p->inAir)
			force = p->AirMoveForce;
		else
			force = p->MoveForce;

		rb->AddImpulse(MoveDir * force);
	}
}

void PlayerMovementSystem::OnJump(InputKey key, InputType type)
{
	std::vector<Entity> entities = world->GetEntities<PlayerMovementComponent, RigidBodyComponent>();

	for (Entity entity : entities)
	{
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);
		PlayerMovementComponent* p = world->GetComponent<PlayerMovementComponent>(entity);

		if (!p->inAir)
		{
			rb->AddImpulse(VECTOR3_UP * p->JumpForce);
		}
	}
}



void PlayerMovementSystem::BindEvents()
{
	inputReceiver = new InputReceiver();

	inputReceiver->AddButtonBinding(this, &PlayerMovementSystem::OnJump, "Jump", InputTypes::BUTTON_PRESSED);
}
