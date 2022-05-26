#pragma region INCLUDE
#include "Game.h"
#include <codecvt>
#include <locale>
#include <algorithm>
#pragma endregion

CGame::CGame()
{
	_application = new CApplication();
	_time = new CTime();
	_graphics = new CGraphics();
	_audio = new CAudio();
	_input = new CInput();
}

CGame::~CGame()
{
	if (_input != nullptr)
	{
		delete _input;
		_input = nullptr;
	}

	if (_audio != nullptr)
	{
		delete _audio;
		_audio = nullptr;
	}

	if (_graphics != nullptr)
	{
		delete _graphics;
		_graphics = nullptr;
	}

	if (_time != nullptr)
	{
		delete _time;
		_time = nullptr;
	}

	if (_application != nullptr)
	{
		delete _application;
		_application = nullptr;
	}
}

#pragma region Game loop

void CGame::Load(HINSTANCE hInstance, std::string gameDataPath)
{
	/* Read file */
	pugi::xml_document gameDataDoc;
	gameDataDoc.load_file(gameDataPath.c_str());

	/* Game settings */
	pugi::xml_node gameSettingsNode = gameDataDoc.child("GameData").child("GameSettings");
	_framePerSecond = gameSettingsNode.attribute("framePerSecond").as_uint();
	_cameraBuffer = gameSettingsNode.attribute("cameraBuffer").as_float();

	/* Framework */
	_application->CreateGameWindow
	(
		hInstance,
		std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
			gameSettingsNode.attribute("title").as_string()
		),
		gameSettingsNode.attribute("windowWidth").as_uint(),
		gameSettingsNode.attribute("windowHeight").as_uint()
	);

	_graphics->Initialize(
		_application->GetWindow()
	);

	_audio->Initialize(
		_application->GetWindow()
	);

	_input->Initialize(
		_application->GetInstance(),
		_application->GetWindow(),
		this
	);


	/* Button */
	for (pugi::xml_node buttonNode = gameDataDoc.child("GameData").child("Button");
		buttonNode;
		buttonNode = buttonNode.next_sibling("Button"))
		BindKey(buttonNode.attribute("keyCode").as_int());


	/* Texture */
	for (pugi::xml_node textureNode = gameDataDoc.child("GameData").child("Texture");
		textureNode;
		textureNode = textureNode.next_sibling("Texture"))
		_graphics->LoadTexture(
			textureNode.attribute("id").as_uint(),
			std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
				textureNode.attribute("source").as_string()
			)
		);

	/* Scene */
	for (pugi::xml_node sceneNode = gameDataDoc.child("GameData").child("Scene");
		sceneNode;
		sceneNode = sceneNode.next_sibling("Scene"))
		_scenes[sceneNode.attribute("id").as_uint()] 
		= sceneNode.attribute("source").as_string();
	
	PlayScene(gameSettingsNode.attribute("startScene").as_uint());
}

void CGame::Run(HINSTANCE hInstance, std::string gameDataPath)
{
	Load(hInstance, gameDataPath);

	float msPerFrame = 1000.0f / _framePerSecond;
	float elapsedMs = 0;
	bool done = false;

	_time->Start();
	while (!done)
	{
		_time->Tick();
		elapsedMs += _time->GetElapsedMs();

		SceneLoading();

		if (elapsedMs >= msPerFrame)
		{
			_input->Process();
			Update(elapsedMs);
			Render();
			Purge();
			elapsedMs = 0;
		}
		else Sleep(DWORD(msPerFrame - elapsedMs));

		done = _application->HandleMessage();
	}
	Shutdown();
}

void CGame::Shutdown()
{
	for (auto gameObjectContainer : _gameObjects)
		gameObjectContainer.second->Destroy();

	Purge();

	_graphics->Shutdown();
	_audio->Shutdown();
	_input->Shutdown();
}

void CGame::Update(float elapsedMs)
{
	_activeQueue.clear();

	for (auto gameObjectContainer : _gameObjects)
	{
		if (!gameObjectContainer.second->IsDestroyed())
		{
			_activeQueue.push_back(gameObjectContainer.second);
		}
	}

	for (auto &gameObject : _activeQueue)
	{
		gameObject->Update(elapsedMs);
		UpdateGrid(gameObject->GetId());
	}
}

void CGame::Render()
{
	std::sort(_activeQueue.begin(), _activeQueue.end(), CGameObject::CompareLayer);

	_graphics->GetDevice()->ClearRenderTargetView(
		_graphics->GetRenderTargetView(),
		D3DXCOLOR(200.0f / 255, 200.0f / 255, 255.0f / 255, 0.0f)
	);
	_graphics->GetSpriteHandler()->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	_graphics->GetDevice()->OMSetBlendState(_graphics->GetBlendStateAlpha(), NewBlendFactor, 0xffffffff);

	for (auto &gameObject : _activeQueue)
		gameObject->Render();

	_graphics->GetSpriteHandler()->End();
	_graphics->GetSwapChain()->Present(0, 0);
}

void CGame::Purge()
{
	for (auto it = _gameObjects.begin(); it != _gameObjects.end();)
	{
		if (it->second->IsDestroyed())
		{
			RemoveGrid(it->first);

			_dictionary.erase(it->second->GetName());

			delete it->second;
			it->second = nullptr;

			it = _gameObjects.erase(it);
		}
		else it++;
	}
}

#pragma endregion

#pragma region Button

void CGame::KeyState()
{
	_previousButtonState = _currentButtonState;
}


void CGame::OnKeyUp(int keyCode)
{
	_currentButtonState[keyCode] = false;
}


void CGame::OnKeyDown(int keyCode)
{
	_currentButtonState[keyCode] = true;
}


void CGame::BindKey(int keyCode)
{
	_previousButtonState[keyCode] = false;
	_currentButtonState[keyCode] = false;
}


bool CGame::IsKeyDown(int keyCode)
{
	return _currentButtonState[keyCode];
}


bool CGame::IsKeyPressed(int keyCode)
{
	return
		!_previousButtonState[keyCode]
		&& _currentButtonState[keyCode];
}


bool CGame::IsKeyReleased(int keyCode)
{
	return
		_previousButtonState[keyCode]
		&& !_currentButtonState[keyCode];
}

#pragma endregion

#pragma region Scene

void CGame::PlayScene(unsigned int nextSceneID)
{
	if (_scenes.find(nextSceneID) == _scenes.end())
		return;

	_nextSceneID = nextSceneID;
	_sceneLoading = true;
}

void CGame::SceneLoading()
{
	if (!_sceneLoading) return;

	for (auto gameObjectContainer : _gameObjects)
		gameObjectContainer.second->Destroy();

	Purge();

	LoadScene(_scenes[_nextSceneID]);
	_sceneLoading = false;
}

void CGame::LoadScene(std::string sceneDataPath)
{
	_nextGameObjectID = 0;

	pugi::xml_document sceneDoc;
	sceneDoc.load_file(sceneDataPath.c_str());

	_gridWidth = sceneDoc.child("Scene").attribute("gridWidth").as_int();
	_gridHeight = sceneDoc.child("Scene").attribute("gridHeight").as_int();

	for (auto gameObjectNode = sceneDoc.child("Scene").child("GameObject");
		gameObjectNode;
		gameObjectNode = gameObjectNode.next_sibling("GameObject")) {
		Create(
			gameObjectNode.attribute("actor").as_uint(),
			gameObjectNode.attribute("name").as_string(),
			gameObjectNode.attribute("source").as_string(),
			gameObjectNode.attribute("x").as_float(),
			gameObjectNode.attribute("y").as_float(),
			gameObjectNode.attribute("gx").as_int(),
			gameObjectNode.attribute("gy").as_int(),
			gameObjectNode.attribute("layer").as_int()
		);
	}
}

#pragma endregion

#pragma region GameObject

void CGame::Add(pGameObject gameObject)
{
	_gameObjects[gameObject->GetId()] = gameObject;
	_dictionary[gameObject->GetName()] = gameObject->GetId();
	AddGrid(gameObject->GetId());
}

pGameObject CGame::Get(unsigned int id)
{
	if (_gameObjects.find(id) != _gameObjects.end())
		return _gameObjects[id];
	else
		return nullptr;
}

pGameObject CGame::Get(std::string name)
{
	if (_dictionary.find(name) != _dictionary.end())
		return _gameObjects[_dictionary[name]];
	else
		return nullptr;
}

std::vector<pGameObject> CGame::GetActives()
{
	return _activeQueue;
}

#pragma endregion

#pragma region Grid

void CGame::AddGrid(unsigned int gameObjectId)
{
	int gridX = 0;
	int gridY = 0;
	_gameObjects[gameObjectId]->GetGrid(gridX, gridY);
	auto cell = std::make_pair(gridX, gridY);

	auto it = _grid.find(cell);
	if (it == _grid.end())
	{
		_grid[cell] = { gameObjectId };
	}
	else
	{
		_grid[cell].push_back(gameObjectId);
	}
}

void CGame::RemoveGrid(unsigned int gameObjectId)
{
	int gridX = 0;
	int gridY = 0;
	_gameObjects[gameObjectId]->GetGrid(gridX, gridY);
	auto cell = std::make_pair(gridX, gridY);

	std::vector<unsigned int>& it = _grid.find(cell)->second;
	it.erase(
		std::remove(it.begin(), it.end(), gameObjectId),
		it.end()
	);
}

void CGame::UpdateGrid(unsigned int gameObjectId)
{
	auto gameObject = _gameObjects[gameObjectId];

	float x = 0;
	float y = 0;
	gameObject->GetPosition(x, y);

	int gx = 0;
	int gy = 0;
	gameObject->GetGrid(gx, gy);

	int newGx = 0;
	int newGy = 0;

	if (int(x) > (_gridWidth / 2))
	{
		newGx = (int(x - (_gridWidth / 2)) / _gridWidth) + 1;
	}
	else if (int(x) < (-_gridWidth / 2))
	{
		newGx = (int(x + (_gridWidth / 2)) / _gridWidth) - 1;
	}

	if (int(y) > (_gridHeight / 2))
	{
		newGy = (int(y - (_gridHeight / 2)) / _gridHeight) + 1;
	}
	else if (int(y) < (-_gridHeight / 2))
	{
		newGy = (int(y + (_gridHeight / 2)) / _gridHeight) - 1;
	}

	if (newGx != gx || newGy != gy)
	{
		RemoveGrid(gameObjectId);
		gameObject->SetGrid(newGx, newGy);
		AddGrid(gameObjectId);
	}
}

std::vector<pGameObject> CGame::GetLocal(unsigned int gameObjectId)
{
	std::vector<unsigned int> local;
	std::vector<pGameObject> gameObjects;

	int gridX = 0;
	int gridY = 0;
	_gameObjects[gameObjectId]->GetGrid(gridX, gridY);

	/* left-top */
	auto cell = std::make_pair(gridX - 1, gridY + 1);
	for (auto gameObject : _grid[cell])
	{
		local.push_back(gameObject);
	}

	/* top */
	cell = std::make_pair(gridX, gridY + 1);
	for (auto gameObject : _grid[cell])
	{
		local.push_back(gameObject);
	}

	/* right-top */
	cell = std::make_pair(gridX + 1, gridY + 1);
	for (auto gameObject : _grid[cell])
	{
		local.push_back(gameObject);
	}

	/* left */
	cell = std::make_pair(gridX - 1, gridY);
	for (auto gameObject : _grid[cell])
	{
		local.push_back(gameObject);
	}

	/* center */
	cell = std::make_pair(gridX, gridY);
	for (auto gameObject : _grid[cell])
	{
		local.push_back(gameObject);
	}

	/* right */
	cell = std::make_pair(gridX + 1, gridY);
	for (auto gameObject : _grid[cell])
	{
		local.push_back(gameObject);
	}

	/* left-bottom */
	cell = std::make_pair(gridX - 1, gridY - 1);
	for (auto gameObject : _grid[cell])
	{
		local.push_back(gameObject);
	}

	/* bottom */
	cell = std::make_pair(gridX, gridY - 1);
	for (auto gameObject : _grid[cell])
	{
		local.push_back(gameObject);
	}

	/* right-bottom */
	cell = std::make_pair(gridX + 1, gridY - 1);
	for (auto gameObject : _grid[cell])
	{
		local.push_back(gameObject);
	}

	for (auto gameObject : local)
	{
		if (gameObject != gameObjectId
			&& !_gameObjects[gameObject]->IsDestroyed())
		{
			gameObjects.push_back(_gameObjects[gameObject]);
		}
	}

	return gameObjects;
}

#pragma endregion
