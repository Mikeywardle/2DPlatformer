#include "PlayerMovementSystem.h"

#include <Inputs/InputData.h>
#include <Core/World.h>
#include <Maths/Transform.h>

#include <Physics/RigidBody.h>
#include <Physics/PhysicsSystem.h>

#include <Components/PlayerMovementComponent.h>
#include <Components/PlayerTags.h>

#include <Camera/CameraComponent.h>

#include <Config/TestInputConfig.h>

PlayerMovementSystem::PlayerMovementSystem(World* world)
	:System(world)
{

}

PlayerMovementSystem::~PlayerMovementSystem()
{
}

void PlayerMovementSystem::OnFrame(float deltaTime)
{

}

void PlayerMovementSystem::OnInput(const float deltaTime, const InputData* inputData)
{
	if (inputData->GetInputValue(TestConfigInputId::Jump, InputTypes::BUTTON_RELEASED))
		Jump();

	CheckPlayersOnGround();
	ProcessMovementInputs(inputData);
	ProcessJumps();


	const Vector2 Delta = inputData->MouseDelta;

	ForEntities(world, PlayerMovementComponent, Transform)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(entity);

		const Vector3 currentRotation = transform->GetRotation();

		Vector3 NewRotation = currentRotation;

		NewRotation.y += Delta.x * pmc->RotationSpeed * deltaTime;
		NewRotation.z -= -Delta.y * pmc->RotationSpeed * deltaTime;

		if (NewRotation.z > 89.0f)
			NewRotation.z = 89.0f;
		if (NewRotation.z < -89.0f)
			NewRotation.z = -89.0f;

		transform->SetRotation(NewRotation);
	}
}

void PlayerMovementSystem::CheckPlayersOnGround()
{
	PhysicsSystem* physicsSystem = world->GetPhysicsSystem();

	std::vector<Entity> entities = world->GetEntities<PlayerMovementComponent, Transform>();

	for (Entity entity : entities)
	{
		Transform* transform = world->GetComponent<Transform>(entity);
		PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(entity);

		const Vector3 RayStart = transform->GetPosition();
		const Vector3 RayEnd = RayStart + Vector3::Down;

		RaycastingResult result = physicsSystem->CastRay(RayStart, RayEnd, entity);

		pmc->OnGround = result.hasHit;

		if (result.hasHit)
		{	
			pmc->NumberOfJumps = 0;
		}
	}
}

void PlayerMovementSystem::ProcessJumps()
{
	std::vector<Entity> entities = world->GetEntities<CurrentPossesedPlayer, PlayerMovementComponent, RigidBodyComponent>();

	for (Entity entity : entities)
	{
		PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(entity);

		if (pmc->ToJump)
		{
			RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);

			rb->AddImpulse(Vector3::Up * pmc->JumpForce);
			pmc->NumberOfJumps += 1;
			pmc->ToJump = false;
		}
	}
}

void PlayerMovementSystem::ProcessMovementInputs(const InputData* inputData)
{
	bool ForwardPressed = inputData->GetInputValue(TestConfigInputId::Up, InputTypes::BUTTON_IS_DOWN);
	bool BackPressed = inputData->GetInputValue(TestConfigInputId::Down, InputTypes::BUTTON_IS_DOWN);
	bool LeftPressed = inputData->GetInputValue(TestConfigInputId::Left, InputTypes::BUTTON_IS_DOWN);
	bool RightPressed = inputData->GetInputValue(TestConfigInputId::Right, InputTypes::BUTTON_IS_DOWN);


	ForEntities(world, CurrentPossesedPlayer, PlayerMovementComponent, RigidBodyComponent, Transform)
	{
		Transform* t = world->GetComponent<Transform>(entity);
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);
		PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(entity);

		Vector3 PlayerForward = t->GetForward();
		Vector3 PlayerRight= t->GetRight();

		PlayerForward.y = 0;
		PlayerRight.y = 0;

		PlayerForward = Vector3::Normalize(PlayerForward);
		PlayerRight = Vector3::Normalize(PlayerRight);

		const Vector3 MovementUnitVector = Vector3::Normalize((PlayerForward * (ForwardPressed - BackPressed)) + (PlayerRight * (RightPressed - LeftPressed)));

		Vector3 MovementImpulse;

		if (pmc->OnGround)
		{
			MovementImpulse = MovementUnitVector * pmc->MovementSpeed;
		}
		else
		{
			MovementImpulse = MovementUnitVector * pmc->AirMovementSpeed;
		}

		rb->AddImpulse(MovementImpulse);
	}

}

void PlayerMovementSystem::Jump()
{
	std::vector<Entity> entities = world->GetEntities<CurrentPossesedPlayer, PlayerMovementComponent>();

	for (Entity entity : entities)
	{
		PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(entity);

		if (pmc->NumberOfJumps < pmc->MaxNumberOfJumps)
		{
			pmc->ToJump = true;
		}
	}


}


