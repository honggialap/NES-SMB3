#pragma region INCLUDE
#include "Model/SuperMarioBros3.h"
#pragma endregion

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	auto game = new CSuperMarioBros3();
	game->Run(hInstance, "Data/GameData.xml");
	delete game;
	game = nullptr;

	return 0;
}