#pragma once
#include <Windows.h>

class Timer
{
public:

	double countsPerSecond = 0.0;
	__int64 StartCounter = 0;

	int frameCount = 0;
	int fps = 0;

	__int64 oldFrameTime = 0;
	double frameTime;

	void StartTimer();
	double GetTime();
	double GetFrameTime();
	void RunTimer();
};

