#pragma once

#include <ctime>

class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	float GetElapsedTime();
	void Stop();
private:
	clock_t begin;
	bool running = false;
};

