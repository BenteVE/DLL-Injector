#include <Windows.h>
#include <iostream>

DWORD WINAPI testThread(PVOID base)
{

    printf("Hello from inside thread.\n");

    FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        FILE* stream; // An out parameter that will point to the reopened stream when the function returns.

        // Open console for debugging
        if (AllocConsole())
        {
            freopen_s(&stream, "CONOUT$", "w", stdout);
            SetConsoleTitle(L"Console");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("DLL loaded.\n");
        }
        CreateThread(nullptr, NULL, testThread, hModule, NULL, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
