#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:
	// Constructor
	Timer();

	void Start();
	void Stop();
	void Continue();

	Uint32 Read();

private:
	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
	Uint32  pausedTimeStart;
	Uint32  pausedTimeStop;
	Uint32  totalPausedTime;
};

#endif //__TIMER_H__