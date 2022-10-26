#pragma once

#include <Windows.h>
#include <iostream>
#include <filesystem>

#include "MH/MinHook.h"

#define DIRECTORY "C:\\Dumps"

namespace Globals {
	inline HMODULE hModule = nullptr;
	inline bool shouldAllocateConsole = false;
}