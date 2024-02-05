#include "Console.h"

Console console;

DWORD WINAPI testThread(PVOID base) {
	fprintf(console.stream, "DLL loaded.\n");
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (!console.open()) {
			// Indicate DLL loading failed
			return FALSE;
		}
		CreateThread(nullptr, NULL, testThread, hModule, NULL, nullptr);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
