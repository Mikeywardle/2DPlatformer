#pragma once

#include <Maths/Color.h>

struct DirectionalLightComponent
{

	DirectionalLightComponent() = default;
	DirectionalLightComponent(Color color, float Strength);

	float strength;
	Color color;
};
