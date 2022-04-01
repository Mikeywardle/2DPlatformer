#include "CollisionShapes.h"

CollisionAABB::CollisionAABB(Vector3 Position, Vector3 HalfLimits)
{
	this->Position = Position;
	this->HalfLimits = HalfLimits;
}

CollisionSphere::CollisionSphere(float Radius, Vector3 Position)
{
	this->Radius = Radius;
	this->Position = Position;
}
