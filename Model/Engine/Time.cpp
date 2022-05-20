#pragma region INCLUDE
#include "Time.h"
#pragma endregion

void CTime::Start()
{
	_elapsedMs = 0.0f;
	_totalElapsedMs = 0.0f;

	_startTimeStamp = _clock.now();
	_previousTimeStamp = _startTimeStamp;
	_currentTimeStamp = _startTimeStamp;
}

void CTime::Tick()
{
	_previousTimeStamp = _currentTimeStamp;
	_currentTimeStamp = _clock.now();

	_elapsedMs = std::chrono::duration_cast<
		std::chrono::duration<float, std::milli>
	>(_currentTimeStamp - _previousTimeStamp).count();

	_totalElapsedMs = std::chrono::duration_cast<
		std::chrono::duration<float, std::milli>
	>(_currentTimeStamp - _startTimeStamp).count();
}
