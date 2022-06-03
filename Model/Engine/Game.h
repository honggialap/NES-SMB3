#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#pragma region INCLUDE
#include "Application.h"
#include "Time.h"
#include "Graphics.h"
#include "Input.h"
#include "Audio.h"
#include "pugixml.hpp"
#include "GameObject.h"

#include <map>
#include <vector>
#include <unordered_map>
#pragma endregion

class CGame : public IKeyHandler
{
public:
	CGame();
	~CGame();

	/* Framework */
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

	/* Game loop */
protected:
	unsigned int _framePerSecond = 0;

public:
	void Load(HINSTANCE hInstance, std::string gameDataPath);
	void Run(HINSTANCE hInstance, std::string gameDataPath);
	void Shutdown();
	
	void Update(float elapsedMs);
	void Render();
	void Purge();

	/* Camera */
protected:
	float _cameraX = 0;
	float _cameraY = 0;
	float _cameraBuffer = 0;

public:
	float GetCameraBuffer() { return _cameraBuffer; }
	void GetCamera(float& x, float& y) { x = _cameraX; y = _cameraY; }
	void MoveCameraTo(float x, float y) { _cameraX = x; _cameraY = y; }
	void MoveCameraBy(float x, float y) { _cameraX += x; _cameraY += y; }

	/* Button */
protected:
	std::map<int, bool> _currentButtonState;
	std::map<int, bool> _previousButtonState;

public:
	virtual void KeyState();
	virtual void OnKeyUp(int keyCode);
	virtual void OnKeyDown(int keyCode);

	void BindKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyPressed(int keyCode);
	bool IsKeyReleased(int keyCode);

	/* Scene */
protected:
	std::map<unsigned int, std::string> _scenes;
	bool _sceneLoading = false;
	unsigned int _nextSceneID = 0;

public:
	void PlayScene(unsigned int nextSceneID);
	void SceneLoading();
	void LoadScene(std::string sceneDataPath);

	/* Game Object */
protected:
	std::unordered_map<unsigned int, pGameObject> _gameObjects;
	std::unordered_map<std::string, unsigned int> _dictionary;
	std::vector<pGameObject> _activeQueue;
	std::map<std::pair<int, int>, std::vector<unsigned int>> _grid;
	unsigned int _nextGameObjectID = 0;
	int _gridWidth = 0;
	int _gridHeight = 0;

public:
	virtual pGameObject Create(
		unsigned int actorID, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY,
		unsigned int layer
	) = 0;
	void Add(pGameObject gameObject);
	pGameObject Get(unsigned int id);
	pGameObject Get(std::string name);
	std::vector<pGameObject> GetActives();

	void AddGrid(unsigned int gameObjectId);
	void RemoveGrid(unsigned int gameObjectId);
	void UpdateGrid(unsigned int gameObjectId);
	std::vector<pGameObject> GetLocal(unsigned int gameObjectId);
};
typedef CGame* pGame;

#endif // !__GAME_H__
