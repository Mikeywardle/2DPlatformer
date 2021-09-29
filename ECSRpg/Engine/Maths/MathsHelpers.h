#pragma once

template<typename T>
T ClampValue(T value, T max, T min)
{
	if (value > max)
		return max;
	else if (value < min)
		return min;
	else
		return value;
}