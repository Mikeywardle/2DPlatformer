#include "CollisionResult.h"

CollisionResult FlipCollisionResult(CollisionResult result)
{
	result.normal *= -1;

	return result;
}
