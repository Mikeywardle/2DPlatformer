#include "Timer.h"

void Timer::StartTimer()
{
	startTime = std::chrono::high_resolution_clock::now();
}

double Timer::StopTimer()
{
	auto endTime = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
}
