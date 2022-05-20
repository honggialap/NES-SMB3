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
}

CGame::~CGame()
{
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
		_time->Tick();
		elapsedMs += _time->GetElapsedMs();

		if (elapsedMs >= msPerFrame)
		{
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

	return true;
}

void CGame::Shutdown()
{
	_graphics->Shutdown();
}
