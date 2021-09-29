#include "CollisionResult.h"

void FlipCollisionResult(CollisionResult& result)
{
	result.normal *= -1;
}
