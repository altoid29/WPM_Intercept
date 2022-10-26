#pragma once

#include "Includes.hpp"

// --------------------------------------------------------------------------------------------------------------------------

namespace HookRelated 
{
	typedef BOOL(WINAPI* WriteProcessMemoryFn)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);
	typedef BOOL(NTAPI* NtWriteVirtualMemoryFn)(HANDLE, PVOID, PVOID, ULONG, PULONG);

	BOOL WINAPI WriteProcessMemoryDetour(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);
	BOOL NTAPI NtWriteVirtualMemoryDetour(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG BufferSize, PULONG NumberOfBytesWritten);

	inline WriteProcessMemoryFn originalWriteProcessMemory = nullptr;
	inline NtWriteVirtualMemoryFn originalNtWriteVirtualMemory = nullptr;
}

// --------------------------------------------------------------------------------------------------------------------------
