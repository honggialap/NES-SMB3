#pragma once
#ifndef __TIME_H__
#define __TIME_H__

#pragma region INCLUDE
#include <chrono>
#pragma endregion

class CTime
{
private:
	std::chrono::steady_clock _clock;
	std::chrono::steady_clock::time_point _startTimeStamp;
	std::chrono::steady_clock::time_point _currentTimeStamp;
	std::chrono::steady_clock::time_point _previousTimeStamp;

	float _elapsedMs = 0.0f;
	float _totalElapsedMs = 0.0f;

public:
	float GetElapsed() { return _elapsedMs; }
	float GetTotalElapsed() { return _totalElapsedMs; }

public:
	void Start();
	void Tick();
};
typedef CTime pTime;

#endif // !__TIME_H__