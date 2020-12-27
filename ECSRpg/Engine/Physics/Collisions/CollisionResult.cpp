#include "CollisionResult.h"

void FlipCollisionResult(CollisionResult& result)
{
	Entity temp = result.entityA;
	result.entityA = result.entityB;
	result.entityB = temp;
	result.normal *= -1;
}
