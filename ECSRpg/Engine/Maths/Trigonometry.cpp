#include "Trigonometry.h"
#include <math.h>

#define TABLE_SIZE 2000
float SINE_LOOKUP_TABLE[TABLE_SIZE];

float DegreesToRadians(float degrees)
{
	return degrees * (PI / 180.f);
}

float RadiansToDegrees(float radians)
{
	return radians * (180.f / PI);
}

void GenerateLookupTable()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		SINE_LOOKUP_TABLE[i] = sin(PI*(i-TABLE_SIZE)/TABLE_SIZE);
	}
}

float sinT(float x)
{
	return 0;
}

float cosT(float x)
{
	return sinT(x + (PI/4));
}
