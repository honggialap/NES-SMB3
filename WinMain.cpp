#pragma region INCLUDE
#include "Model/Engine/Application.h"
#pragma endregion

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	auto app = new CApplication();

	app->CreateGameWindow(hInstance, L"Super Mario Bros 3", 512, 480);
	while (!app->HandleMessage());

	delete app;
}