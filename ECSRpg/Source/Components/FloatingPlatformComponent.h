#pragma once

#include <Maths/Vector3.h>

struct FloatingPlatformComponent
{
	Vector3 a = Vector3::Zero;
	Vector3 b = Vector3::Zero;

	float moveSpeed = 0.0f;
	bool towardsA = false;
};
