#include "PlayerMovementComponent.h"

PlayerMovementComponent::PlayerMovementComponent( float JumpForce, float MoveForce, float AirMoveForce)
{
	this->JumpForce = JumpForce;
	this->MoveForce = MoveForce;
	this->AirMoveForce = AirMoveForce;
}
