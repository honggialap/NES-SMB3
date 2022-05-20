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

class CGame : public IKeyHandler
{
protected:
	pApplication _application = nullptr;
	pTime _time = nullptr;
	pGraphics _graphics = nullptr;
	pAudio _audio = nullptr;
	pInput _input = nullptr;

	float _frameRate = 0.0f;

	std::map<int, bool> _currentButtonState;
	std::map<int, bool> _previousButtonState;

	float _cameraX = 0;
	float _cameraY = 0;

	float _cameraBoundOffset = 0;

	std::unordered_map<unsigned int, std::string> _scenes;
	unsigned int _nextPlaySceneID = 0;
	bool _sceneLoading = false;

	unsigned int _nextGameObjectID = 0;
	std::unordered_map<unsigned int, pGameObject> _gameObjects;
	std::unordered_map<std::string, unsigned int> _dictionary;

	std::vector<unsigned int> _updateQueue;
	std::vector<pGameObject> _renderQueue;

	std::map<std::pair<int, int>, std::vector<unsigned int>> _grid;
	int _gridWidth = 0;
	int _gridHeight = 0;

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

	virtual void KeyState() {
		_previousButtonState = _currentButtonState;
	}

	virtual void OnKeyUp(int keyCode) {
		if (_currentButtonState.find(keyCode) != _currentButtonState.end())
			_currentButtonState[keyCode] = false;
	}
	virtual void OnKeyDown(int keyCode) {
		if (_currentButtonState.find(keyCode) != _currentButtonState.end())
			_currentButtonState[keyCode] = true;
	}

	void BindKey(int keyCode) {
		_previousButtonState[keyCode] = false;
		_currentButtonState[keyCode] = false;
	}

	bool IsKeyDown(int keyCode) {
		return _currentButtonState[keyCode];
	}
	bool IsKeyPressed(int keyCode) {
		return !_previousButtonState[keyCode] && _currentButtonState[keyCode];
	}
	bool IsKeyReleased(int keyCode) {
		return _previousButtonState[keyCode] && !_currentButtonState[keyCode];
	}

	void GetCamera(float& x, float& y) { x = _cameraX; y = _cameraY; }
	void MoveCameraTo(float x, float y) { _cameraX = x; _cameraY = y; }
	void MoveCameraBy(float x, float y) { _cameraX += x; _cameraY += y; }

	float GetCameraBoundOffset() { return _cameraBoundOffset; }

	void PlayScene(unsigned int nextPlaySceneID);
	void Loading();
	void LoadScene(std::string sceneDataPath);

	void Update(float elapsedMs);
	void Render();
	void Purge();

	void AddGameObject(pGameObject gameObject);
	pGameObject GetGameObject(unsigned int gameObjectID);
	pGameObject GetGameObject(std::string gameObjectName);
	std::vector<unsigned int> GetActives();

	void AddGrid(unsigned int gameObjectID);
	void RemoveGrid(unsigned int gameObjectID);
	void UpdateGrid(unsigned int gameObjectID);
	std::vector<pGameObject> GetLocal(unsigned int gameObjectID);

	virtual pGameObject Create(
		unsigned int actorID, std::string name, std::string source,
		float posX, float posY,
		int gridX, int gridY,
		unsigned int layer
	) = 0;
};
typedef CGame* pGame;

#endif // !__GAME_H__
