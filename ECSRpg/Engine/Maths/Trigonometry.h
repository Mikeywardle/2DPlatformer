#pragma once

static float PI = 3.1415926535;
static float TWO_PI = PI * 2;
static float RADIANS_IN_360_DEGREES = 6.28318530718;

float DegreesToRadians(float degrees);
float RadiansToDegrees(float radians);

void GenerateLookupTable();

float sinT(float x);
float cosT(float x);