#pragma region INCLUDE
#include "Model/Engine/Game.h"
#pragma endregion

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	auto game = new CGame();
	game->Run(hInstance, "Data/GameData.xml");

	delete game;
	game = nullptr;
}