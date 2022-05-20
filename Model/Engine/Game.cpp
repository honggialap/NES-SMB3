#pragma region INCLUDE
#include "Game.h"
#include "Ultilities.h"
#include "pugixml.hpp"
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

void CGame::Run(
	HINSTANCE hInstance,
	std::string gameDataFilePath
)
{
	if (!Load(hInstance, gameDataFilePath))
	{
		Shutdown();
		return;
	}

	float msPerFrame = 1000.0f / _frameRate;
	float elapsedMs = 0.0f;

	_time->Start();

	while (!_application->HandleMessage())
	{
		Loading();

		_time->Tick();
		elapsedMs += _time->GetElapsedMs();

		if (elapsedMs >= msPerFrame)
		{
			_input->ProcessKeyboard();
			Update(elapsedMs);
			Render();
			Purge();
			elapsedMs = 0.0f;
		}
		else
		{
			Sleep(DWORD(msPerFrame - elapsedMs));
		}
	}

	Shutdown();
}

bool CGame::Load(
	HINSTANCE hInstance,
	std::string gameDataFilePath
)
{
	bool result = false;

	pugi::xml_document gameDataDoc;
	gameDataDoc.load_file(gameDataFilePath.c_str());

	auto gameSettingsNode = gameDataDoc.child("GameData").child("GameSettings");
	_frameRate = gameSettingsNode.attribute("fps").as_float();

	result = _application->CreateGameWindow(
		hInstance,
		std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
			gameSettingsNode.attribute("title").as_string()
		),
		gameSettingsNode.attribute("width").as_uint(),
		gameSettingsNode.attribute("height").as_uint()
	);
	if (!result) return false;

	result = _graphics->Initialize(
		_application->GetWindow()
	);
	if (!result) return false;

	for (auto textureNode = gameDataDoc.child("GameData").child("Texture");
		textureNode;
		textureNode = textureNode.next_sibling("Texture"))
	{
		_graphics->LoadTexture(
			textureNode.attribute("ID").as_uint(),
			std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
				textureNode.attribute("source").as_string()
			)
		);
	}

	result = _audio->Initialize(
		_application->GetWindow()
	);
	if (!result) return false;

	result = _input->Initialize(
		_application->GetInstance(),
		_application->GetWindow(),
		this
	);
	if (!result) return false;

	auto playerSettingsNode = gameDataDoc.child("GameData").child("PlayerSettings");
	_cameraBoundOffset = playerSettingsNode.attribute("cameraBoundOffset").as_float();

	for (auto buttonNode = playerSettingsNode.child("Button");
		buttonNode;
		buttonNode = buttonNode.next_sibling("Button"))
	{
		BindKey(
			buttonNode.attribute("keyCode").as_int()
		);
	}

	return true;
}

void CGame::Shutdown()
{
	for (auto gameObjectContainer : _gameObjects)
		gameObjectContainer.second->Destroy();

	Purge();

	_input->Shutdown();
	_audio->Shutdown();
	_graphics->Shutdown();
}

void CGame::PlayScene(unsigned int nextPlaySceneID)
{
	if (_scenes.find(nextPlaySceneID) == _scenes.end())
		return;

	_nextPlaySceneID = nextPlaySceneID;
	_sceneLoading = true;
}

void CGame::Loading()
{
	if (!_sceneLoading) return;

	for (auto gameObjectContainer : _gameObjects)
		gameObjectContainer.second->Destroy();

	Purge();

	LoadScene(_scenes[_nextPlaySceneID]);
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
			gameObjectNode.attribute("actorID").as_uint(),
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

void CGame::Update(float elapsedMs)
{
	_updateQueue.clear();
	_renderQueue.clear();

	for (auto gameObjectContainer : _gameObjects)
	{
		if (!gameObjectContainer.second->IsDestroyed())
		{
			_updateQueue.push_back(gameObjectContainer.first);
			_renderQueue.push_back(gameObjectContainer.second);
		}
	}

	for (auto gameObjectID : _updateQueue)
	{
		_gameObjects[gameObjectID]->Update(elapsedMs);
		UpdateGrid(gameObjectID);
	}
}

void CGame::Render()
{
	std::sort(
		_renderQueue.begin(),
		_renderQueue.end(),
		CGameObject::CompareLayer
	);

	_graphics->GetDevice()->ClearRenderTargetView(
		_graphics->GetRenderTargetView(),
		D3DXCOLOR(200.0f / 255, 200.0f / 255, 255.0f / 255, 0.0f)
	);
	_graphics->GetSpriteHandler()->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	_graphics->GetDevice()->OMSetBlendState(_graphics->GetBlendStateAlpha(), NewBlendFactor, 0xffffffff);

	for (auto& gameObject : _renderQueue)
		gameObject->Render();

	_graphics->GetSpriteHandler()->End();
	_graphics->GetSwapChain()->Present(0, 0);
}

void CGame::Purge()
{
	for (auto gameObjectContainer = _gameObjects.begin();
		gameObjectContainer != _gameObjects.end();
		)
	{
		if (gameObjectContainer->second->IsDestroyed())
		{
			RemoveGrid(gameObjectContainer->first);
			_dictionary.erase(gameObjectContainer->second->GetName());

			delete gameObjectContainer->second;
			gameObjectContainer->second = nullptr;

			gameObjectContainer = _gameObjects.erase(gameObjectContainer);
		}
		else
		{
			gameObjectContainer++;
		}
	}
}

void CGame::AddGameObject(pGameObject gameObject)
{
	_gameObjects[gameObject->GetID()] = gameObject;
	_dictionary[gameObject->GetName()] = gameObject->GetID();
	AddGrid(gameObject->GetID());
}

pGameObject CGame::GetGameObject(unsigned int gameObjectID)
{
	if (_gameObjects.find(gameObjectID) != _gameObjects.end())
		return _gameObjects[gameObjectID];

	return nullptr;
}

pGameObject CGame::GetGameObject(std::string gameObjectName)
{
	if (_dictionary.find(gameObjectName) != _dictionary.end())
		return GetGameObject(_dictionary[gameObjectName]);

	return nullptr;
}

std::vector<unsigned int> CGame::GetActives()
{
	return _updateQueue;
}

void CGame::AddGrid(unsigned int gameObjectID)
{
	int gridX = 0;
	int gridY = 0;

	_gameObjects[gameObjectID]->GetGrid(gridX, gridY);
	auto cell = std::make_pair(gridX, gridY);

	if (_grid.find(cell) == _grid.end())
		_grid[cell] = { gameObjectID };
	else
		_grid[cell].push_back(gameObjectID);
}

void CGame::RemoveGrid(unsigned int gameObjectID)
{
	int gridX = 0;
	int gridY = 0;

	_gameObjects[gameObjectID]->GetGrid(gridX, gridY);
	auto cell = std::make_pair(gridX, gridY);

	std::vector<unsigned int>& it = _grid.find(cell)->second;
	it.erase(
		std::remove(it.begin(), it.end(), gameObjectID),
		it.end()
	);
}

void CGame::UpdateGrid(unsigned int gameObjectID)
{
	auto gameObject = _gameObjects[gameObjectID];

	float x = 0;
	float y = 0;
	gameObject->GetPosition(x, y);

	int gx = 0;
	int gy = 0;
	gameObject->GetGrid(gx, gy);

	int newGx = 0;
	int newGy = 0;

	if (int(x) > (_gridWidth / 2))
		newGx = (int(x - (_gridWidth / 2)) / _gridWidth) + 1;
	else if (int(x) < (-_gridWidth / 2))
		newGx = (int(x + (_gridWidth / 2)) / _gridWidth) - 1;


	if (int(y) > (_gridHeight / 2))
		newGy = (int(y - (_gridHeight / 2)) / _gridHeight) + 1;
	else if (int(y) < (-_gridHeight / 2))
		newGy = (int(y + (_gridHeight / 2)) / _gridHeight) - 1;


	if (newGx != gx || newGy != gy)
	{
		RemoveGrid(gameObjectID);
		gameObject->SetGrid(newGx, newGy);
		AddGrid(gameObjectID);
	}
}

std::vector<pGameObject> CGame::GetLocal(unsigned int gameObjectID)
{
	std::vector<unsigned int> local;
	std::vector<pGameObject> gameObjects;

	int gridX = 0;
	int gridY = 0;
	_gameObjects[gameObjectID]->GetGrid(gridX, gridY);

	/* left-top */
	auto cell = std::make_pair(gridX - 1, gridY + 1);
	for (auto gameObject : _grid[cell])
		local.push_back(gameObject);

	/* top */
	cell = std::make_pair(gridX, gridY + 1);
	for (auto gameObject : _grid[cell])
		local.push_back(gameObject);

	/* right-top */
	cell = std::make_pair(gridX + 1, gridY + 1);
	for (auto gameObject : _grid[cell])
		local.push_back(gameObject);

	/* left */
	cell = std::make_pair(gridX - 1, gridY);
	for (auto gameObject : _grid[cell])
		local.push_back(gameObject);

	/* center */
	cell = std::make_pair(gridX, gridY);
	for (auto gameObject : _grid[cell])
		local.push_back(gameObject);

	/* right */
	cell = std::make_pair(gridX + 1, gridY);
	for (auto gameObject : _grid[cell])
		local.push_back(gameObject);

	/* left-bottom */
	cell = std::make_pair(gridX - 1, gridY - 1);
	for (auto gameObject : _grid[cell])
		local.push_back(gameObject);

	/* bottom */
	cell = std::make_pair(gridX, gridY - 1);
	for (auto gameObject : _grid[cell])
		local.push_back(gameObject);

	/* right-bottom */
	cell = std::make_pair(gridX + 1, gridY - 1);
	for (auto gameObject : _grid[cell])
		local.push_back(gameObject);

	for (auto gameObject : local)
	{
		if (gameObject != gameObjectID
			&& !_gameObjects[gameObject]->IsDestroyed())
			gameObjects.push_back(_gameObjects[gameObject]);
	}

	return gameObjects;
}
