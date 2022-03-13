#include "PlayerMovementComponent.h"

PlayerMovementComponent::PlayerMovementComponent(float InMovementSpeed, float InAirMovementSpeed, float InJumpForce, uint8 InMaxNumberOfJumps, float InRotationSpeed)
	:MaxNumberOfJumps(InMaxNumberOfJumps)
	, MovementSpeed(InMovementSpeed)
	, AirMovementSpeed(InAirMovementSpeed)
	, JumpForce(InJumpForce)
	, RotationSpeed(InRotationSpeed)
{
}
