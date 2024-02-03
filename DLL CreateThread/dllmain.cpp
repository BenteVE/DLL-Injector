#include <Windows.h>
#include <iostream>

// Pointer to the opened stream
FILE* stream;

DWORD WINAPI testThread(PVOID base) {
	fprintf(stream, "DLL loaded.\n");
	return 0;
}

// Open console for debugging
BOOL openConsole() {
	
	if (!AllocConsole())
	{
		return FALSE;
	}
	SetConsoleTitle(TEXT("Console"));
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	errno_t err;
	// Reopen stream "stdout" and assign it to the console:
	err = freopen_s(&stream, "CONOUT$", "w", stdout);

	if (err != 0 || stream == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

void clean() {
	if (stream != NULL) {
		fclose(stream);
	}
	FreeConsole();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (!openConsole()) {
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
		clean();
		break;
	}
	return TRUE;
}
