#pragma once
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#pragma region INCLUDE
#include <Windows.h>
#include <string>
#pragma endregion

class CApplication
{
private:
	HINSTANCE _hInstance = NULL;
	HWND _hWnd = NULL;

public:
	HINSTANCE GetInstance() { return _hInstance; }
	HWND GetWindow() { return _hWnd; }

	HWND CreateGameWindow(HINSTANCE hInstance, std::wstring title, unsigned int width, unsigned int height);
	void Exit();
	bool HandleMessage();
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
typedef CApplication* pApplication;

#endif // !__APPLICATION_H__
