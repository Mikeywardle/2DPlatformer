#pragma once

#include <Maths/MathsTypes.h>

struct PlayerMovementComponent
{
	PlayerMovementComponent() = default;
	PlayerMovementComponent(float InMovementSpeed, float InAirMovementSpeed, float InJumpForce, uint8 MaxNumberOfJumps);

	uint8 NumberOfJumps = 0;
	uint8 MaxNumberOfJumps;
	bool OnGround = false;

	float MovementSpeed;
	float AirMovementSpeed;
	float JumpForce;

};
