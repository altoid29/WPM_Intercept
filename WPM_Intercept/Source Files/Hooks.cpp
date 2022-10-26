#include "../Header Files/Includes.hpp"
#include "../Header Files/Hooks.hpp"

// --------------------------------------------------------------------------------------------------------------------------

BOOL WINAPI HookRelated::WriteProcessMemoryDetour(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten)
{
    // File name which is set to the address of the dumped address.
    char buffer[9999];
    sprintf_s(buffer, "%s\\WPM_0x%p%s", DIRECTORY, lpBaseAddress, ".bin");

    // Create a file and use the name from the buffer.
    const HANDLE hFile = CreateFile(buffer, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile != nullptr)
    {
        if (lpBaseAddress != nullptr && nSize > 8 && lpBuffer > 0x0)
        {
            // Write the content to the file with WriteFile.
            try
            {
#ifdef _WIN64
                WriteFile(hFile, lpBuffer, nSize, reinterpret_cast<LPDWORD>(lpNumberOfBytesWritten), nullptr);
#else
                WriteFile(hFile, lpBuffer, nSize, lpNumberOfBytesWritten, nullptr);
#endif
                std::cout << "[WPM] Wrote " << nSize << " bytes (0x" << std::hex << nSize << ")" << std::dec << " from address " << "0x" << lpBaseAddress << std::endl;
            }
            catch (std::exception& error)
            {

            }

            // Don't leave any unused handles opened.
            // If we do, we won't be able to open the files.
            CloseHandle(hFile);
        }
    }
    else
        std::cout << "[!] Invalid file handle (0x" << hFile << ")" << std::endl;

    return originalWriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}

// --------------------------------------------------------------------------------------------------------------------------

BOOL NTAPI HookRelated::NtWriteVirtualMemoryDetour(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG BufferSize, PULONG NumberOfBytesWritten)
{
    // File name with the current address.
    char buffer[9999];
    sprintf_s(buffer, "%s\\NT_0x%p%s", DIRECTORY, BaseAddress, ".bin");

    // Create a file and use the name from the buffer.
    const HANDLE hFile = CreateFile(buffer, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile != nullptr)
    {
        if (BaseAddress != nullptr && BufferSize > 8 && Buffer > 0x0)
        {
            try
            {
#ifdef _WIN64
                // Casting needed?
                WriteFile(hFile, Buffer, BufferSize, reinterpret_cast<LPDWORD>(NumberOfBytesWritten), nullptr);
#else
                WriteFile(hFile, Buffer, BufferSize, NumberOfBytesWritten, nullptr);
#endif

                std::cout << "[NT] Wrote " << BufferSize << " bytes (0x" << std::hex << BufferSize << ")" << std::dec << " from address " << "0x" << BaseAddress << std::endl;
            }
            catch (std::exception& error)
            {

            }

            // Close the handle so we can open the files.
            CloseHandle(hFile);
        }
        else
            std::cout << "[!] Invalid file handle (0x" << hFile << ")" << std::endl;

        return originalNtWriteVirtualMemory(ProcessHandle, BaseAddress, Buffer, BufferSize, NumberOfBytesWritten);
    }

    // --------------------------------------------------------------------------------------------------------------------------
