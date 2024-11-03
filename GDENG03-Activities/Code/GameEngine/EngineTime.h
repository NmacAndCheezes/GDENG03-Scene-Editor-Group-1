#pragma once
#include <chrono>
#include <ctime>


class Window;

class EngineTime
{
public:
	EngineTime(const EngineTime&) = delete;
	EngineTime& operator=(const EngineTime&) = delete;

	static void Initialize();
	static double GetDeltaTime();
	static double GetFixedDeltaTime();
	static double GetTimeSinceStartup();
	static void SetTimeScale(float newScale);

private:
	EngineTime() {};
	~EngineTime() {};

	static void LogStartFrame();
	static void LogEndFrame();


private:
	static EngineTime* sharedInstance;

	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;

	double deltaTime;
	double fixedDeltaTime;
	double timeSinceStartup;
	float timeScale;

	friend class Window;
};