#pragma region INCLUDE
#include "Model/Engine/Application.h"
#include "Model/Engine/Time.h"
#pragma endregion

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	auto app = new CApplication();
	auto time = new CTime();
	time->Start();

	app->CreateGameWindow(
		hInstance, 
		L"Super Mario Bros 3",
		512, 480
	);
	while (!app->HandleMessage())
	{
		time->Tick();
		if (time->GetTotalElapsed() > 5000)
		{
			app->Exit();
		}
	}

	delete time;
	delete app;
}