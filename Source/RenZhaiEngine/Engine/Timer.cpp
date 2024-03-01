#include "Timer.h"
#include <windows.h>

FTimer::FTimer()
{
	__int64 Frequency = 0ll;
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);

	SecondsPerCount = 1.0 / (double)Frequency;
}

void FTimer::Reset()
{
	__int64 CurrentCounter = 0ll;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentCounter);

	CurrentTime = CurrentCounter;
	PrevTime = CurrentCounter;
}

void FTimer::Tick()
{
	__int64 CurrentCounter = 0ll;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentCounter);
	CurrentTime = CurrentCounter;

	DeltaTime = (CurrentTime - PrevTime) * SecondsPerCount;

	PrevTime = CurrentCounter;

	if (DeltaTime < 0.0)
	{
		DeltaTime = 0.0;
	}
}

float FTimer::GetDeltaTime() const
{
	return (float)DeltaTime;
}
