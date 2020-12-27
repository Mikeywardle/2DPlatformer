#pragma once

#include <chrono>

class Timer
{
public:
	void StartTimer();
	double StopTimer();

private:
	std::chrono::steady_clock::time_point startTime;
};
