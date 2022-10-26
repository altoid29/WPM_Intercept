#include "../Header Files/Includes.hpp"
#include "../Header Files/Hooks.hpp"

// --------------------------------------------------------------------------------------------------------------------------

BOOL WINAPI HookRelated::WriteProcessMemoryDetour(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten)
{
    // File name which is set to the address of the dumped address.
    char Buffer[9999];
    sprintf_s(Buffer, "%s\\0x%p", DIRECTORY, lpBaseAddress);

    // Create a file and use the name from the buffer.
    const HANDLE FileHandle = CreateFile(Buffer, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (FileHandle)
    {
        // You may need to modify the size to your needs.
        // I found that 8 works fine for ignoring false positives.
        if (lpBaseAddress != nullptr && nSize > 8 && lpBuffer > 0x0)
        {
            // Write the content to the file with WriteFile.
            try
            {
#ifdef _WIN64
                WriteFile(FileHandle, lpBuffer, nSize, reinterpret_cast<LPDWORD>(lpNumberOfBytesWritten), nullptr);
#else
                WriteFile(FileHandle, lpBuffer, nSize, lpNumberOfBytesWritten, nullptr);
#endif
                std::cout << "[+] Wrote " << nSize << " bytes (0x" << std::hex << nSize << ")" << std::dec << " from address " << "0x" << lpBaseAddress << std::endl;
            }
            catch (std::exception& error)
            {

            }

            // Don't leave any un-used handles opened.
            // If we do, we won't be able to open the files.
            CloseHandle(FileHandle);
        }
    }
    else
    {
        std::cout << "[!] Invalid file handle" << std::endl;
        FreeLibraryAndExitThread(Globals::ThisModule, -1);
    }

    return OriginalWriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}

// --------------------------------------------------------------------------------------------------------------------------
