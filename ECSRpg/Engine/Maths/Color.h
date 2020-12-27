#pragma once
struct Color
{
	Color() {};
	Color(float r, float g, float b, float a);

	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;
};

static Color COLOR_WHITE = {1,1,1,1};
static Color COLOR_RED = {1,0,0,1};
static Color COLOR_BLUE = {0,0,1,1};
static Color COLOR_GREEN = {0,1,0,1};
static Color COLOR_BLACK = {0,0,0,1};
static Color COLOR_CLEAR = {1,1,1,0};
