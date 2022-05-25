#pragma once
#ifndef __INPUT_H__
#define __INPUT_H__

#pragma region DEFINE
#define DIRECTINPUT_VERSION 0x0800
#pragma endregion

#pragma region INCLUDE
#include <dinput.h>
#pragma endregion

class IKeyHandler
{
public:
	virtual void KeyState() = 0;
	virtual void OnKeyDown(int keyCode) = 0;
	virtual void OnKeyUp(int keyCode) = 0;
};
typedef IKeyHandler* pKeyHandler;

class CInput
{
public:
	static const unsigned int KEYBOARD_BUFFER_SIZE = 1024;
	static const unsigned int KEYBOARD_STATE_SIZE = 256;

private:
	LPDIRECTINPUT8 _directInput = NULL;
	LPDIRECTINPUTDEVICE8 _device = NULL;
	BYTE _keyStates[CInput::KEYBOARD_STATE_SIZE];
	DIDEVICEOBJECTDATA _keyEvents[CInput::KEYBOARD_BUFFER_SIZE];
	pKeyHandler _keyHandler = nullptr;

public:
	void Initialize(HINSTANCE hInstance, HWND hWnd, pKeyHandler keyHandler);
	void Shutdown();

	void Process();
};
typedef CInput* pInput;

#endif // !__INPUT_H__
