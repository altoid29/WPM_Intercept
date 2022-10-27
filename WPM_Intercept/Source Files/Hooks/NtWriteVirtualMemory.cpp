#include "../../Header Files/Includes.hpp"
#include "../../Header Files/Hooks.hpp"

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
                WriteFile(hFile, Buffer, BufferSize, NumberOfBytesWritten, nullptr);

                std::cout << "[NT] Wrote " << BufferSize << " bytes (0x" << std::hex << BufferSize << ")" << std::dec << " from address " << "0x" << BaseAddress << std::endl;
            }
            catch (std::exception& error)
            {

            }

            // Close the handle so we can open the files when we'd like.
            CloseHandle(hFile);
        }
    }

    return originalNtWriteVirtualMemory(ProcessHandle, BaseAddress, Buffer, BufferSize, NumberOfBytesWritten);
}

// --------------------------------------------------------------------------------------------------------------------------