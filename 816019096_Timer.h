#pragma once
#include "816019096_Commons.h"

/*
This timer object was used for benchmarking purposes and left in for future dev.
*/

class Timer {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
public:
	Timer();
	~Timer();

	void Stop();

};