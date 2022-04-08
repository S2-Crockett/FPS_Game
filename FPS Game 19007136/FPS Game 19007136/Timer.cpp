#include "Timer.h"


void Timer::StartTimer()
{
	//Store the counts per second in the variable countsPerSecond using QueryPerformanceFrequency
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	countsPerSecond = double(frequency.QuadPart);

	//Store the current time in counts in the StartCounter variable
	QueryPerformanceCounter(&frequency);
	StartCounter = frequency.QuadPart;
}

double Timer::GetTime()
{
	//Get the current time in counts using QueryPerformanceCounter and store it in the current time variable
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	//minus the current time by the StartCounter variable which returns the amount of counts since the starttimer
	//function was last called. 
	//This value is then divided by the countsPerSecond variable to get the time in seconds since StartTimer was last called.
	return double(currentTime.QuadPart - StartCounter) / countsPerSecond;
}

double Timer::GetFrameTime()
{
	//Similar to the GetTime function but instead of returng time since StartTimer was last called return time sinse
	//GetFrameTime was last called. Running this every frame will result in the time in seconds each frame took to process
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - oldFrameTime;
	oldFrameTime = currentTime.QuadPart;

	if (tickCount < 0.0f)
		tickCount = (__int64)0.0f;

	return float(tickCount) / countsPerSecond;
}

void Timer::RunTimer()
{
	//increase the frame count
	frameCount++;
	//call the gettime function to see if its been more than one second sinse StartTimer was called. 
	if (GetTime() > 1.0f)
	{
		//if true set the fps to the framecount
		fps = frameCount;
		//then reset the framecount to 0
		frameCount = 0;
		//reset the start timer
		StartTimer();
	}
	//set the frametime value to the GetFrameTime function return value which will be the time since it was last called
	//this is the valuse that is used for the game time in any update function
	frameTime = GetFrameTime();
}