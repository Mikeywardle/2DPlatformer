#include "PlayerMovementComponent.h"

PlayerMovementComponent::PlayerMovementComponent(unsigned short TotalJumps, float JumpForce, float MoveForce, float AirMoveForce)
{
	this->TotalJumps = TotalJumps;
	this->JumpForce = JumpForce;
	this->MoveForce = MoveForce;
	this->AirMoveForce = AirMoveForce;
}
