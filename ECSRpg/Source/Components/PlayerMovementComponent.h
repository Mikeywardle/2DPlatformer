#pragma once

#include <Maths/MathsTypes.h>
#include <Maths/Vector3.h>
#include <ecs/Entity.h>

struct PlayerMovementComponent
{
	PlayerMovementComponent() = default;
	PlayerMovementComponent(float InMovementSpeed, float InAirMovementSpeed, float InJumpForce, uint8 MaxNumberOfJumps);

	uint8 NumberOfJumps = 0;
	uint8 MaxNumberOfJumps;
	bool OnGround = false;
	bool ToJump = false;

	float MovementSpeed;
	float AirMovementSpeed;
	float JumpForce;

	float maxWalkAngle;

	Entity cameraEntity;

};

struct PlayerMovementCamera
{
	float RotationSpeed = 0;
};

struct DungeonPlayerMovementComponent
{
	DungeonPlayerMovementComponent() = default;

	bool canMove = true;
	bool isMoving = false;
	bool isTurning = false;

	Vector3 start = Vector3::Zero;
	Vector3 target = Vector3::Zero;
	float currentAlpha = 0.0f;

	float MoveSpeed = 1.5f;
	float RotationSpeed = 1.5f;
};
