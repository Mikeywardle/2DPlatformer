#pragma once

struct PlayerMovementComponent
{
	PlayerMovementComponent() = default;
	PlayerMovementComponent(unsigned short TotalJumps, float JumpForce, float MoveForce, float AirMoveForce);

	bool MoveingLeft;
	bool MovingRight;
	bool inAir;
	unsigned short CurrentJumpsNumber;
	unsigned short TotalJumps;

	float JumpForce;
	float MoveForce;
	float AirMoveForce;
};
