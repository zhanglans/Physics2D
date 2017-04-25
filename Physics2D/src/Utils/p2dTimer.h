#ifndef _P2D_TIMER_H_
#define _P2D_TIMER_H_

#include "p2dSettings.h"

/// Timer for profiling. This has platform specific code and may
/// not work on every platform.
class p2dTimer
{
public:

	/// Constructor
	p2dTimer();

	/// Reset the timer.
	void Reset();

	/// Get the time since construction or the last reset.
	float32 GetMilliseconds() const;

private:

#if defined(_WIN32)
	float64 m_start;
	static float64 s_invFrequency;
#elif defined(__linux__) || defined (__APPLE__)
	unsigned long m_start_sec;
	unsigned long m_start_usec;
#endif
};

#endif
