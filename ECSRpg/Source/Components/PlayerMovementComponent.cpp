#include "PlayerMovementComponent.h"

PlayerMovementComponent::PlayerMovementComponent(float InMovementSpeed, float InAirMovementSpeed, float InJumpForce, uint8 InMaxNumberOfJumps)
	:MaxNumberOfJumps(InMaxNumberOfJumps)
	, MovementSpeed(InMovementSpeed)
	, AirMovementSpeed(InAirMovementSpeed)
	, JumpForce(InJumpForce)
{
}
