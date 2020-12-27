#include "PlayerMovementSystem.h"
#include <Inputs/InputReceiver.h>

#include "../Components/PlayerMovementComponent.h"

#include <Physics/RigidBody.h>

void PlayerMovementSystem::OnFrame(float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<PlayerMovementComponent, RigidBodyComponent>();

	for (Entity entity : entities)
	{

		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);
		PlayerMovementComponent* p = world->GetComponent<PlayerMovementComponent>(entity);

		Vector3 MoveDir = VECTOR3_RIGHT * (p->MovingRight - p->MoveingLeft);
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

		p->inAir = true;

		if (p->CurrentJumpsNumber < p->TotalJumps)
		{
			rb->AddImpulse(VECTOR3_UP * p->JumpForce);
			++p->CurrentJumpsNumber;
		}

	}
}

void PlayerMovementSystem::OnPlayerLand(CollisionEvent collisions)
{

	static const float collisionTolerance = .25f;

	for (CollisionResult result : collisions.results)
	{
		PlayerMovementComponent* playerMovement = nullptr;
		Vector3 groundDirection;

		if (world->HasComponent<PlayerMovementComponent>(result.entityA))
		{
			playerMovement = world->GetComponent<PlayerMovementComponent>(result.entityA);
			groundDirection = VECTOR3_UP;
		}
		else if (world->HasComponent<PlayerMovementComponent>(result.entityB))
		{
			playerMovement = world->GetComponent<PlayerMovementComponent>(result.entityB);
			groundDirection = VECTOR3_DOWN;
		}


		if (playerMovement == nullptr)
			continue;

		if (Vector3::DotProduct(result.normal, groundDirection) >= collisionTolerance)
		{
			playerMovement->CurrentJumpsNumber = 0;
			playerMovement->inAir = false;
		}		
	}
}

void PlayerMovementSystem::OnLeftPressed(InputKey key, InputType type)
{
	std::vector<PlayerMovementComponent>* components = world->GetComponents<PlayerMovementComponent>();

	for (PlayerMovementComponent& p : *components)
	{
		p.MoveingLeft = true;
	}
}

void PlayerMovementSystem::OnLeftReleased(InputKey key, InputType type)
{
	std::vector<PlayerMovementComponent>* components = world->GetComponents<PlayerMovementComponent>();

	for (PlayerMovementComponent& p : *components)
	{
		p.MoveingLeft = false;
	}
}

void PlayerMovementSystem::OnRightPressed(InputKey key, InputType type)
{
	std::vector<PlayerMovementComponent>* components = world->GetComponents<PlayerMovementComponent>();

	for (PlayerMovementComponent& p : *components)
	{
		p.MovingRight = true;
	}
}

void PlayerMovementSystem::OnRightReleased(InputKey key, InputType type)
{
	std::vector<PlayerMovementComponent>* components = world->GetComponents<PlayerMovementComponent>();

	for (PlayerMovementComponent& p : *components)
	{
		p.MovingRight = false;
	}
}

void PlayerMovementSystem::BindEvents()
{
	inputReceiver = new InputReceiver();

	inputReceiver->AddButtonBinding(this, &PlayerMovementSystem::OnJump, "Jump", InputTypes::BUTTON_PRESSED);

	inputReceiver->AddButtonBinding(this, &PlayerMovementSystem::OnLeftPressed, "Left", InputTypes::BUTTON_PRESSED);
	inputReceiver->AddButtonBinding(this, &PlayerMovementSystem::OnLeftReleased, "Left", InputTypes::BUTTON_RELEASED);

	inputReceiver->AddButtonBinding(this, &PlayerMovementSystem::OnRightPressed, "Right", InputTypes::BUTTON_PRESSED);
	inputReceiver->AddButtonBinding(this, &PlayerMovementSystem::OnRightReleased, "Right", InputTypes::BUTTON_RELEASED);
}
