#pragma once

#include <Maths/MathsTypes.h>

struct EntityColour
{
	static const uint8 Black = 7;
	static const uint8 Blue = 2;
	static const uint8 Red = 3;
	static const uint8 Yellow = 5;
	static const uint8 Purple = 6; // Blue * Red
	static const uint8 Green = 10; // Blue * Yellow
	static const uint8 Orange = 15; // Red * Yellow
	static const uint8 White = 30; // Blue * Red * Yellow

	static bool IsColourType();
};
