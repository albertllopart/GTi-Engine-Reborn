// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
	pausedTimeStart = SDL_GetTicks();
}

void Timer::Continue()
{
	running = true;
	pausedTimeStop = SDL_GetTicks();
	totalPausedTime = pausedTimeStop - pausedTimeStart;
	started_at = started_at + totalPausedTime;
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}


