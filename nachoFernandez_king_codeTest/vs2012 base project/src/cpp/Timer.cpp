#include <Timer.h>

Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::Start()
{
	begin = clock();
	running = true;
}

float Timer::GetElapsedTime()
{
	if (!running)
		return 0;

	return static_cast<float>((clock() - begin) / CLOCKS_PER_SEC);
}

void Timer::Stop()
{
	begin = 0;
	running = false;
}
