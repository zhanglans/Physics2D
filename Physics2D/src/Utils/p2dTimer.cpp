#include "p2dTimer.h"

#if defined(_WIN32)

float64 p2dTimer::s_invFrequency = 0.0f;

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

p2dTimer::p2dTimer()
{
	LARGE_INTEGER largeInteger;

	if (s_invFrequency == 0.0f)
	{
		QueryPerformanceFrequency(&largeInteger);
		s_invFrequency = float64(largeInteger.QuadPart);
		if (s_invFrequency > 0.0f)
		{
			s_invFrequency = 1000.0f / s_invFrequency;
		}
	}

	QueryPerformanceCounter(&largeInteger);
	m_start = float64(largeInteger.QuadPart);
}

void p2dTimer::Reset()
{
	LARGE_INTEGER largeInteger;
	QueryPerformanceCounter(&largeInteger);
	m_start = float64(largeInteger.QuadPart);
}

float32 p2dTimer::GetMilliseconds() const
{
	LARGE_INTEGER largeInteger;
	QueryPerformanceCounter(&largeInteger);
	float64 count = float64(largeInteger.QuadPart);
	float32 ms = float32(s_invFrequency * (count - m_start));
	return ms;
}

#elif defined(__linux__) || defined (__APPLE__)

#include <sys/time.h>

p2dTimer::p2dTimer()
{
    Reset();
}

void p2dTimer::Reset()
{
    timeval t;
    gettimeofday(&t, 0);
    m_start_sec = t.tv_sec;
    m_start_usec = t.tv_usec;
}

float32 p2dTimer::GetMilliseconds() const
{
    timeval t;
    gettimeofday(&t, 0);
    return 1000.0f * (t.tv_sec - m_start_sec) + 0.001f * (t.tv_usec - m_start_usec);
}

#else

p2dTimer::p2dTimer()
{
}

void p2dTimer::Reset()
{
}

float32 p2dTimer::GetMilliseconds() const
{
	return 0.0f;
}

#endif
