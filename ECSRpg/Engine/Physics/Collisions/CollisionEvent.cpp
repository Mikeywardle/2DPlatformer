#include "CollisionEvent.h"

PhysicsCollisionEnd::PhysicsCollisionEnd(Entity entityA, Entity entityB)
{
	this->entityA = entityA;
	this->entityB = entityB;
}
