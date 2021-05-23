#include "PlayerMovementSystem.h"

#include <Inputs/InputReceiver.h>
#include <Core/World.h>
#include <Core/Transform.h>

#include <Physics/RigidBody.h>
#include <Physics/PhysicsSystem.h>

#include <Components/PlayerMovementComponent.h>
#include <Components/PlayerTags.h>

#include <Camera/CameraComponent.h>

PlayerMovementSystem::PlayerMovementSystem(World* world)
	:System(world)
{
	inputReceiver = new InputReceiver();

	inputReceiver->AddButtonBinding(this, &PlayerMovementSystem::Jump, "Jump", InputTypes::BUTTON_RELEASED);
}

PlayerMovementSystem::~PlayerMovementSystem()
{
	delete inputReceiver;
}

void PlayerMovementSystem::OnFrame(float deltaTime)
{
	CheckPlayersOnGround();
	ProcessMovementInputs();
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
		const Vector3 RayEnd = RayStart + VECTOR3_DOWN;

		RaycastingResult result = physicsSystem->CastRay(RayStart, RayEnd, entity);

		pmc->OnGround = result.hasHit;

		if (result.hasHit)
		{	
			pmc->NumberOfJumps = 0;
		}
	}
}

void PlayerMovementSystem::ProcessMovementInputs()
{
	bool ForwardPressed = inputReceiver->BindingPressed("Forwards");
	bool BackPressed = inputReceiver->BindingPressed("Backwards");
	bool LeftPressed = inputReceiver->BindingPressed("Left");
	bool RightPressed = inputReceiver->BindingPressed("Right");


	Entity CameraEntity = CameraComponent::GetMainCamera();

	Transform* CameraTransfrom = world->GetComponent<Transform>(CameraEntity);
	Vector3 CameraForward = CameraTransfrom->GetForward();
	CameraForward.y = 0;
	CameraForward = Vector3::Normalize(CameraForward);

	Vector3 CameraRight = CameraTransfrom->GetRight();
	CameraRight.y = 0;
	CameraRight = Vector3::Normalize(CameraRight);

	const Vector3 MovementUnitVector = Vector3::Normalize((CameraForward * (ForwardPressed - BackPressed)) + (CameraRight * (RightPressed - LeftPressed)));

	if (Vector3::Magnitude(MovementUnitVector) == 0)
		return;

	std::vector<Entity> entities = world->GetEntities<CurrentPossesedPlayer, PlayerMovementComponent, RigidBodyComponent>();

	for (Entity entity : entities)
	{
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);
		PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(entity);

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

void PlayerMovementSystem::Jump(InputKey key, InputType type)
{
	std::vector<Entity> entities = world->GetEntities<CurrentPossesedPlayer, PlayerMovementComponent, RigidBodyComponent>();

	for (Entity entity : entities)
	{
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);
		PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(entity);

		if (pmc->NumberOfJumps < pmc->MaxNumberOfJumps)
		{
			rb->AddImpulse(VECTOR3_UP * pmc->JumpForce);
			pmc->NumberOfJumps += 1;
		}
	}


}


