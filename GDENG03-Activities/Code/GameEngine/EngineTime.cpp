#include "EngineTime.h"


EngineTime* EngineTime::sharedInstance = NULL;

void EngineTime::Initialize()
{
	sharedInstance = new EngineTime();
	sharedInstance->deltaTime = 0;
	sharedInstance->fixedDeltaTime = 0;
	sharedInstance->timeSinceStartup = 0;
	sharedInstance->timeScale = 1;
}

double EngineTime::GetDeltaTime()
{
	return sharedInstance->deltaTime;
}

double EngineTime::GetFixedDeltaTime()
{
	return sharedInstance->fixedDeltaTime;
}

double EngineTime::GetTimeSinceStartup()
{
	return sharedInstance->timeSinceStartup;
}

void EngineTime::SetTimeScale(float newScale)
{
	if (newScale < 0) return;
	sharedInstance->timeScale = newScale;
}

void EngineTime::LogStartFrame()
{
	sharedInstance->start = std::chrono::system_clock::now();
}

void EngineTime::LogEndFrame()
{
	sharedInstance->end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsedInSeconds = sharedInstance->end - sharedInstance->start;

	if (sharedInstance->timeScale > 0)
	{
		sharedInstance->deltaTime = elapsedInSeconds.count() * (double)sharedInstance->timeScale;
	}
	else
	{
		sharedInstance->deltaTime = 0;
	}

	sharedInstance->fixedDeltaTime = elapsedInSeconds.count(); 
	sharedInstance->timeSinceStartup += elapsedInSeconds.count();
}