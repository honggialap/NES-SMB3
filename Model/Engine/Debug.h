#pragma once
#ifndef __DEBUG_H__
#define __DEBUG_H__

#pragma region INCLUDE
#include <windows.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#pragma endregion

void DebugOut(const wchar_t* fmt, ...);

#endif // !__DEBUG_H__