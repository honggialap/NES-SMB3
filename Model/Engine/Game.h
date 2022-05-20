#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#pragma region INCLUDE
#include "Application.h"
#include "Time.h"
#include "Graphics.h"
#include "Audio.h"
#include "Input.h"
#pragma endregion

class CGame
{
protected:
	pApplication _application = nullptr;
	pTime _time = nullptr;
	pGraphics _graphics = nullptr;
	pAudio _audio = nullptr;
	pInput _input = nullptr;

	float _frameRate = 0.0f;

public:
	pApplication GetApplication() { return _application; }
	pTime GetTime() { return _time; }
	pGraphics GetGraphics() { return _graphics; }
	pAudio GetAudio() { return _audio; }
	pInput GetInput() { return _input; }

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
