#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#pragma region INCLUDE
#include "Application.h"
#include "Time.h"
#pragma endregion

class CGame 
{
protected:
	pApplication _application = nullptr;
	pTime _time = nullptr;

	float _frameRate = 0.0f;

public:
	pApplication GetApplication() { return _application; }
	pTime GetTime() { return _time; }

public:
	CGame();
	~CGame();

	void Run(
		HINSTANCE hInstance, 
		std::string gameDataFilePath
	);
	bool Load(
		HINSTANCE hInstance, 
		std::string gameDataFilePath
	);
	void Shutdown();
};
typedef CGame* pGame;

#endif // !__GAME_H__
