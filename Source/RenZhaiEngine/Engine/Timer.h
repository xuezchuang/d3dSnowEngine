#pragma once

class FTimer
{
public:
	FTimer();

	void Reset();
	void Tick();

	float GetDeltaTime() const;
private:
	double SecondsPerCount;
	double DeltaTime;

	__int64 CurrentTime;
	__int64 PrevTime;

	bool mAppPaused = false;
};