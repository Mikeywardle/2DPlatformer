#include "DungeonPlayerMovementSystem.h"

#include <Inputs/InputData.h>

#include "Config/TestInputConfig.h"

#include <Core/World.h>

#include <Components/PlayerMovementComponent.h>

#include <Maths/Transform.h>

#include <Physics/PhysicsSystem.h>
#include <Physics/Collisions/PhysicsCollisionResult.h>

#include <Data/NeOniCollisionLayers.h>

DungeonPlayerMovementSystem::DungeonPlayerMovementSystem(World* world)
	: System(world)
{
}

void DungeonPlayerMovementSystem::OnInput(const float deltaTime, const InputData* inputData)
{
	bool wPressed = inputData->GetInputValue(TestConfigInputId::Up, InputTypes::BUTTON_IS_DOWN);
	bool sPressed = inputData->GetInputValue(TestConfigInputId::Down, InputTypes::BUTTON_IS_DOWN);

	bool aPressed = inputData->GetInputValue(TestConfigInputId::Left, InputTypes::BUTTON_IS_DOWN);
	bool dPressed = inputData->GetInputValue(TestConfigInputId::Right, InputTypes::BUTTON_IS_DOWN);

	ForEntities(world, DungeonPlayerMovementComponent, Transform)
	{
		DungeonPlayerMovementComponent* dmc = world->GetComponent<DungeonPlayerMovementComponent>(entity);
		Transform* transform = world->GetComponent<Transform>(entity);

		//If not turning or moving receive inputs
		if (!dmc->isTurning && !dmc->isMoving)
		{
			if (aPressed || dPressed)
			{
				//Turn left/Right
				const Vector3 currentRotation = transform->GetRotation();;
				const Vector3 targetRotation = currentRotation + Vector3(0, 90 * (dPressed - aPressed), 0);

				dmc->start = currentRotation;
				dmc->target = targetRotation;
				dmc->isTurning = true;
				dmc->currentAlpha = 0.0f;
			}
			else if (sPressed)
			{
				//Turn 180
				const Vector3 currentRotation = transform->GetRotation();;
				const Vector3 targetRotation = currentRotation + Vector3(0, 180, 0);

				dmc->start = currentRotation;
				dmc->target = targetRotation;
				dmc->isTurning = true;
				dmc->currentAlpha = 0.0f;

			}
			else if (wPressed)
			{
				//Move Forward
				PhysicsSystem* physicsSystem = world->GetPhysicsSystem();

				const Vector3 PlayerPosition = transform->GetPosition();
				const Vector3 PlayerForward = transform->GetForward();
				const Vector3 TargetCellLocation = PlayerPosition + (PlayerForward * 2);

				CollisionAABB aabb = CollisionAABB(TargetCellLocation + (Vector3::Down), Vector3(2, 3, 2));
				std::vector<uint8> environmentLayers = {NeOniCollisionLayers::Environment};

				std::vector<PhysicsCollisionCastResult> boxCastResults = physicsSystem->CastBox(aabb, environmentLayers);

				if (boxCastResults.size() == 0)
				{
					dmc->start = PlayerPosition;
					dmc->target = TargetCellLocation;
					dmc->isMoving = true;
					dmc->currentAlpha = 0.0f;
				}
			}
		}
	}
}

void DungeonPlayerMovementSystem::OnFrame(float deltaTime)
{

	ForEntities(world, DungeonPlayerMovementComponent, Transform)
	{
		DungeonPlayerMovementComponent* dmc = world->GetComponent<DungeonPlayerMovementComponent>(entity);
		Transform* transform = world->GetComponent<Transform>(entity);

		//If not turning or moving receive inputs
		if (dmc->isTurning)
		{
			dmc->currentAlpha = fminf(1.0f, dmc->currentAlpha + (dmc->MoveSpeed * deltaTime));
			const Vector3 currentRotation = transform->GetRotation();
			const Vector3 newRotation = Vector3::Lerp(dmc->start, dmc->target, dmc->currentAlpha);
			transform->SetRotation(newRotation);

			if (dmc->currentAlpha == 1.0f)
			{
				dmc->isTurning = false;
			}
		}
		else if (dmc->isMoving)
		{
			dmc->currentAlpha = fminf(1.0f, dmc->currentAlpha + (dmc->MoveSpeed * deltaTime));
			const Vector3 currentPosition = transform->GetPosition();
			const Vector3 newPosition = Vector3::Lerp(dmc->start, dmc->target, dmc->currentAlpha);
			transform->SetPosition(newPosition);		

			if (dmc->currentAlpha == 1.0f)
			{
				dmc->isMoving = false;
			}
		}
	}
}


