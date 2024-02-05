#include "Console.h"

// Open console for debugging
Console::Console() {
	stream = NULL;
}

Console::~Console() {
	if (stream != NULL) {
		fclose(stream);
	}
	FreeConsole();
}

BOOL Console::open() {

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