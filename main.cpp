#include "Model/SuperMarioBros3.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	auto game = new CSuperMarioBros3();
	game->Load(hInstance, "Data/GameData.xml");
	game->Run();
}