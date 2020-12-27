#pragma once
#include <Maths/Vector3.h>

struct FloatingPlatformComponent
{
	FloatingPlatformComponent() = default;
	FloatingPlatformComponent(Vector3 start,Vector3 end,float speed);

	bool movingForwards = true;
	float speed;
	Vector3 start;
	Vector3 end;

};