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
	std::chrono::steady_clock::time_point _previousTimeStamp;
	std::chrono::steady_clock::time_point _currentTimeStamp;

	float _elapsedMs = 0;
	float _totalElapsedMs = 0;

public:
	float GetElapsedMs() { return _elapsedMs; }
	float GetTotalElapsedMs() { return _totalElapsedMs; }

	void Start();
	void Tick();
};
typedef CTime* pTime;

#endif // __TIME_H__