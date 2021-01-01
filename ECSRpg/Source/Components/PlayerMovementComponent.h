#pragma once

struct PlayerMovementComponent
{
	PlayerMovementComponent() = default;
	PlayerMovementComponent(float JumpForce, float MoveForce, float AirMoveForce);

	bool inAir;

	float JumpForce;
	float MoveForce;
	float AirMoveForce;
};
