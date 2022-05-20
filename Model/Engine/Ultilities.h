#pragma once
#ifndef __ULTILITIES_H__
#define __ULTILITIES_H__

#pragma region INCLUDE
#include <Windows.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#pragma endregion

void DebugOut(
	const wchar_t* fmt,
	...
);

#endif // !__ULTILITIES_H__
