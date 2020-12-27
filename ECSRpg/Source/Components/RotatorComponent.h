#pragma once
#include <Maths/Vector3.h>

struct RotatorComponent
{
	RotatorComponent() = default;

	RotatorComponent(Vector3 deltaPerSecond)
	{
		this->deltaPerSecond = deltaPerSecond;
	}

	Vector3 deltaPerSecond;
};
