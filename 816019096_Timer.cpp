#include "816019096_Timer.h"
#include "816019096_Commons.h"
	

Timer::Timer() {
	m_StartTimePoint = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
	Stop();
}

void Timer::Stop() {
	auto m_endTimePoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_endTimePoint).time_since_epoch().count();

	auto duration = end - start;
	//getting millisec val
	double ms = duration * 0.001;

	std::cout << duration << "us/" << ms << "ms" << std::endl;
}
