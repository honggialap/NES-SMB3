#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#pragma region INCLUDE
#include "Application.h"
#include "Time.h"
#include "Graphics.h"
#include "Audio.h"
#include "Input.h"
#include "GameObject.h"

#include <map>
#include <algorithm>
#pragma endregion

class CGame : public IKeyHandler {
public:
	CGame();
	~CGame();


	/* Framework components */
protected:
	pApplication _application = nullptr;
	pTime _time = nullptr;
	pGraphics _graphics = nullptr;
	pAudio _audio = nullptr;
	pInput _input = nullptr;

public:
	pApplication GetApplication() { return _application; }
	pTime GetTime() { return _time; }
	pGraphics GetGraphics() { return _graphics; }
	pAudio GetAudio() { return _audio; }
	pInput GetInput() { return _input; }


	/* Player components */
protected:
	std::map<int, bool> _currentButtonState;
	std::map<int, bool> _previousButtonState;

	float _cameraX = 0;
	float _cameraY = 0;

	float _cameraBoundOffset = 0;

public:
	virtual void KeyState() {
		_previousButtonState = _currentButtonState;
	}
	virtual void OnKeyUp(int keyCode) {
		if (_currentButtonState.find(keyCode) 
			!= _currentButtonState.end())
			_currentButtonState[keyCode] = false;
	}
	virtual void OnKeyDown(int keyCode) {
		if (_currentButtonState.find(keyCode)
			!= _currentButtonState.end())
			_currentButtonState[keyCode] = true;
	}

	bool IsKeyDown(int keyCode) {
		return _currentButtonState[keyCode];
	}
	bool IsKeyPressed(int keyCode) {
		return !_previousButtonState[keyCode]
			&& _currentButtonState[keyCode];
	}
	bool IsKeyReleased(int keyCode) {
		return _previousButtonState[keyCode]
			&& !_currentButtonState[keyCode];
	}

	void GetCamera(float& x, float& y) {
		x = _cameraX;
		y = _cameraY;
	}
	void MoveCameraTo(float x, float y) {
		_cameraX = x;
		_cameraY = y;
	}
	void MoveCameraBy(float x, float y) {
		_cameraX += x;
		_cameraY += y;
	}

	float GetCameraBoundOffset() {
		return _cameraBoundOffset;
	}
	void SetCameraBoundOffset(float cameraBoundOffset) {
		_cameraBoundOffset = cameraBoundOffset;
	}

protected:
	void BindKey(int keyCode) {
		_previousButtonState[keyCode] = false;
		_currentButtonState[keyCode] = false;
	}


	/* Scene managing */
protected:
	std::unordered_map<unsigned int, std::string> _scenes;
	unsigned int _nextPlaySceneID = 0;
	bool _sceneLoading = false;

public:
	void PlayScene(unsigned int nextPlaySceneID);

protected:
	void Loading();
	void LoadScene(std::string sceneDataPath);


	/* Game object managing */
protected:
	std::unordered_map<unsigned int, pGameObject> _gameObjects;
	std::unordered_map<std::string, unsigned int> _dictionary;
	std::vector<pGameObject> _activeQueue;
	unsigned int _nextGameObjectID = 0;

	std::map<std::pair<int, int>, std::vector<unsigned int>> _grid;
	int _gridWidth = 0;
	int _gridHeight = 0;

public:
	pGameObject GetGameObject(unsigned int gameObjectID);
	pGameObject GetGameObject(std::string gameObjectName);

	std::vector<pGameObject> GetActives();
	std::vector<pGameObject> GetLocal(unsigned int gameObjectID);

	virtual pGameObject Create(
		unsigned int actorID, std::string name, std::string source,
		float posX, float posY,
		int gridX, int gridY,
		unsigned int layer
	) = 0;

protected:
	void AddGameObject(pGameObject gameObject);

	void AddGrid(unsigned int gameObjectID);
	void RemoveGrid(unsigned int gameObjectID);
	void UpdateGrid(unsigned int gameObjectID);


	/* Game loop */
protected:
	float _frameRate = 0.0f;

public:
	void Run(HINSTANCE hInstance, std::string gameDataFilePath);

protected:
	bool Load(HINSTANCE hInstance, std::string gameDataFilePath);
	void Shutdown();
	
	void Update(float elapsedMs);
	void Render();
	void Purge();
};
typedef CGame* pGame;

#endif // !__GAME_H__
