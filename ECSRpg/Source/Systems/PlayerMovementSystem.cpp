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


	const Vector2 Delta = inputData->MouseDelta;

	ForEntities(world, PlayerMovementComponent, SceneTransformComponent)
	{
		SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(entity);
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

	ForEntities(world, PlayerMovementComponent, SceneTransformComponent, RigidBodyComponent)
	{
		SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(entity);
		PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(entity);
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);

		const Vector3 RayStart = transform->GetPosition();
		const Vector3 RayEnd = RayStart + Vector3::Down;

		Ray ray = Ray(RayStart, RayEnd);
		RaycastingResult result = physicsSystem->CastRay(ray, entity);

		pmc->OnGround = result.hasHit;

		if (result.hasHit)
		{	
			pmc->NumberOfJumps = 0;
			//rb->frictionAllowed = true;
		}
	}
}

void PlayerMovementSystem::ProcessJumps()
{
	ForEntities(world, CurrentPossesedPlayer, PlayerMovementComponent, RigidBodyComponent)
	{
		PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(entity);

		if (pmc->ToJump)
		{
			RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);

			rb->AddImpulse(Vector3::Up * pmc->JumpForce);
			//rb->frictionAllowed = false;
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


	ForEntities(world, CurrentPossesedPlayer, PlayerMovementComponent, RigidBodyComponent, SceneTransformComponent)
	{
		SceneTransformComponent* t = world->GetComponent<SceneTransformComponent>(entity);
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

void PlayerMovementSystem::OnPlayerCollided(CollisionEvent& collisionEvent)
{
	//Set On ground to false for all
	std::vector<PlayerMovementComponent>* components =  world->GetComponents<PlayerMovementComponent>();

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


