#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#pragma region INCLUDE
#include "Framework/Application.h"
#include "Framework/Time.h"
#include "Framework/Graphics.h"
#include "Framework/Input.h"
#include "Framework/Audio.h"
#include "Framework/pugixml.hpp"
#include "GameObject.h"
#include <map>
#include <vector>
#include <unordered_map>
#pragma endregion

struct CScene
{
	bool _play;
	bool _load;
	std::string _source;
	std::vector<unsigned int> _gameObjects;

	CScene(std::string source)
	{
		_source = source;
		_load = false;
		_play = false;
	}
};
typedef CScene* pScene;

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
	std::vector<unsigned int> _updateQueue;
	std::vector<pGameObject> _renderQueue;

public:
	void Load(HINSTANCE hInstance, std::string gameDataPath);
	void Run();
	void Update(float elapsedMs);
	void Render();
	void Shutdown();

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
	std::map<unsigned int, pScene> _scenes;
	bool _load = false;
	unsigned int _startScene = 0;

public:
	void PlayScene(unsigned int id);
	void StopScene(unsigned int id);
	void LoadScene();

	/* Game Object */
protected:
	unsigned int nextId = 0;
	std::unordered_map<unsigned int, pGameObject> _gameObjects;
	std::unordered_map<std::string, unsigned int> _dictionary;

public:
	virtual pGameObject Create(
		pScene scene,
		unsigned int actorID, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY,
		unsigned int layer
	) = 0;
	void Add(pGameObject gameObject);
	pGameObject Get(unsigned int id);
	pGameObject Get(std::string name);
	std::vector<unsigned int> GetActives();
	void Purge();

	/* Grid */
protected:
	int _gridWidth = 0;
	int _gridHeight = 0;
	std::map<std::pair<int, int>, std::vector<unsigned int>> _grid;

public:
	void AddGrid(unsigned int gameObjectId);
	void RemoveGrid(unsigned int gameObjectId);
	void UpdateGrid(unsigned int gameObjectId);
	std::vector<pGameObject> GetLocal(unsigned int gameObjectId);
};
typedef CGame* pGame;

#endif // !__GAME_H__
