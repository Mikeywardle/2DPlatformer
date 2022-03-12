#include "Timer.h"

void Timer::StartTimer()
{
	startTime = std::chrono::steady_clock::now();
}

double Timer::StopTimer()
{
	auto endTime = std::chrono::steady_clock::now();

	 std::chrono::duration<double> elapsed_seconds = endTime - startTime;
	return elapsed_seconds.count() * 1000;
}
