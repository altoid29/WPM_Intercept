#include "../Header Files/Includes.hpp"
#include "../Header Files/Hooks.hpp"

// --------------------------------------------------------------------------------------------------------------------------

void Start()
{
    if (Globals::shouldAllocateConsole)
    {
        FILE* f{};
        AllocConsole();
        freopen_s(&f, "CONOUT$", "w", stdout);
        system("cls");
    }

    const bool directoryExists = std::filesystem::is_directory(DIRECTORY);

    // Create a directory to store the dumps.
    if (!directoryExists)
        CreateDirectoryA(DIRECTORY, nullptr);

    const LPVOID address = (LPVOID)(GetProcAddress(GetModuleHandleA("kernel32.dll"), "WriteProcessMemory"));
    const LPVOID address2 = (LPVOID)(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWriteVirtualMemory"));

    if (MH_Initialize() != MH_OK)
    {
        std::cout << "[!] Failed to initialize MH, Unloading" << std::endl;
        FreeLibraryAndExitThread(Globals::hModule, -1);
    }

    if (MH_CreateHook(address2, &HookRelated::NtWriteVirtualMemoryDetour, reinterpret_cast<LPVOID*>(&HookRelated::originalNtWriteVirtualMemory)) != MH_OK)
    {
        std::cout << "[!] Failed to hook NtWriteVirtualMemory, Unloading" << std::endl;
        FreeLibraryAndExitThread(Globals::hModule, -1);
    }

    if (MH_CreateHook(address, &HookRelated::WriteProcessMemoryDetour, reinterpret_cast<LPVOID*>(&HookRelated::originalWriteProcessMemory)) != MH_OK)
    {
        std::cout << "[!] Failed to hook WriteProcessMemory, Unloading" << std::endl;
        FreeLibraryAndExitThread(Globals::hModule, -1);
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        std::cout << "[!] Failed to enable hook, Unloading" << std::endl;
        FreeLibraryAndExitThread(Globals::hModule, -1);
    }

    std::cout << "[+] Initialization succeeded" << std::endl;
}

// --------------------------------------------------------------------------------------------------------------------------

BOOL APIENTRY DllMain(HMODULE hModule, uintptr_t attachReason, LPVOID lpReserved)
{
    if (attachReason == DLL_PROCESS_ATTACH)
    {
        Globals::hModule = hModule;

        DisableThreadLibraryCalls(hModule);

        MessageBoxA(nullptr, "Do you want to create a console window?\nNote: If the process already has a console created press 'No'", "Confirmation", MB_YESNO) == IDYES
            ? Globals::shouldAllocateConsole = true
            : Globals::shouldAllocateConsole = false;

        // Initialize.
        Start();
    }

    return TRUE;
}

// --------------------------------------------------------------------------------------------------------------------------