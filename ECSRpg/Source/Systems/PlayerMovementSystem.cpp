#include "PlayerMovementSystem.h"

#include <Inputs/InputData.h>
#include <Core/World.h>
#include <Core/SceneTransformComponents.h>

#include <Physics/RigidBody.h>
#include <Physics/PhysicsSystem.h>

#include <Physics/Collisions/CollisionEvent.h>

#include <Components/PlayerMovementComponent.h>
#include <Components/PlayerTags.h>

#include <Maths/Trigonometry.h>

#include <Camera/CameraComponent.h>

#include <Config/TestInputConfig.h>

PlayerMovementSystem::PlayerMovementSystem(World* world)
	:System(world)
{
	world->AddListener<CollisionEvent&>(this, &PlayerMovementSystem::OnPlayerCollided);
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

	//CheckPlayersOnGround();
	ProcessMovementInputs(inputData);
	ProcessJumps();

	Vector2 Delta = inputData->MouseDelta;

	world->ForEntities<PlayerMovementComponent, SceneTransformComponent>
		(
			[&](const Entity entity, PlayerMovementComponent* pmc, SceneTransformComponent* playerTransform)
			{
				SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(pmc->cameraEntity);
				const PlayerMovementCamera* moveCamera = world->GetComponent<PlayerMovementCamera>(pmc->cameraEntity);

				const Vector3 currentRotation = transform->GetLocalRotation();
				const Vector3 currentPlayerRotation = playerTransform->GetRotation();

				Vector3 NewRotation = currentRotation;
				Vector3 NewPlayerRotation = currentPlayerRotation;

				const Vector3 PlayerUp = playerTransform->GetUp();

				if (Vector3::DotProduct(PlayerUp, Vector3::Up) < 0)
				{
					Delta.x *= -1.0f;
					Delta.y *= -1.0f;
				}

				NewPlayerRotation.y += Delta.x * moveCamera->RotationSpeed * deltaTime;
				NewPlayerRotation.z -= -Delta.y * moveCamera->RotationSpeed * deltaTime;

				if (NewPlayerRotation.z > 89.0f)
					NewPlayerRotation.z = 89.0f;
				if (NewPlayerRotation.z < -89.0f)
					NewPlayerRotation.z = -89.0f;

				transform->SetLocalRotation(NewRotation);
				playerTransform->SetRotation(NewPlayerRotation);
			}
	);
}

void PlayerMovementSystem::ProcessJumps()
{
	world->ForEntities<CurrentPossesedPlayer, PlayerMovementComponent, RigidBodyComponent, SceneTransformComponent>
		(
			[&](const Entity entity, CurrentPossesedPlayer* player, PlayerMovementComponent* pmc, RigidBodyComponent* rb, SceneTransformComponent* transform)
			{
				if (pmc->ToJump)
				{
					rb->AddImpulse(transform->GetUp() * pmc->JumpForce);
					pmc->NumberOfJumps += 1;
					pmc->ToJump = false;
				}
			}
		);
}

void PlayerMovementSystem::ProcessMovementInputs(const InputData* inputData)
{
	bool ForwardPressed = inputData->GetInputValue(TestConfigInputId::Up, InputTypes::BUTTON_IS_DOWN);
	bool BackPressed = inputData->GetInputValue(TestConfigInputId::Down, InputTypes::BUTTON_IS_DOWN);
	bool LeftPressed = inputData->GetInputValue(TestConfigInputId::Left, InputTypes::BUTTON_IS_DOWN);
	bool RightPressed = inputData->GetInputValue(TestConfigInputId::Right, InputTypes::BUTTON_IS_DOWN);


	world->ForEntities<CurrentPossesedPlayer, PlayerMovementComponent, RigidBodyComponent, SceneTransformComponent>
		(
			[&](const Entity entity, CurrentPossesedPlayer* player, PlayerMovementComponent* pmc, RigidBodyComponent* rb, SceneTransformComponent* transform)
			{
				SceneTransformComponent* t = world->GetComponent<SceneTransformComponent>(pmc->cameraEntity);

				Vector3 PlayerForward = t->GetForward();
				Vector3 PlayerRight = t->GetRight();

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
	);
}

void PlayerMovementSystem::OnPlayerCollided(CollisionEvent& collisionEvent)
{
	//Set On ground to false for all
	std::vector<PlayerMovementComponent>* components =  world->GetComponents<PlayerMovementComponent>();

	if (components)
	{
		for (PlayerMovementComponent& pmc : *components)
		{
			pmc.OnGround = false;
		}

		for (const PhysicsCollisionResult& result : collisionEvent.results)
		{
			Entity playerEntity = NO_ENTITY;

			if (world->HasComponent<PlayerMovementComponent>(result.entityA))
			{
				playerEntity = result.entityA;
			}
			else if (world->HasComponent<PlayerMovementComponent>(result.entityB))
			{
				playerEntity = result.entityB;
			}

			if (playerEntity != NO_ENTITY)
			{
				PlayerMovementComponent* pmc = world->GetComponent< PlayerMovementComponent>(playerEntity);
				SceneTransformComponent* stc = world->GetComponent< SceneTransformComponent>(playerEntity);

				const Vector3 down = stc->GetUp() * -1.0f;
				const Vector3 position = stc->GetPosition();

				const Vector3 positionDiff = Vector3::Normalize(result.collisionInfo.collisionLocation - position);

				const float angleDiff = acosf(Vector3::DotProduct(positionDiff, down));

				if (RadiansToDegrees(angleDiff) < pmc->maxWalkAngle)
				{
					pmc->OnGround = true;
					pmc->NumberOfJumps = 0;
				}

			}
		}
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


