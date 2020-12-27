#pragma once
#include "CollisionResult.h"

enum class ColliderType
{
	AABB,
	OBB,
	SPHERE,
	CAPSULE,
	PLANE
};

struct Collider
{
	ColliderType type;
};

