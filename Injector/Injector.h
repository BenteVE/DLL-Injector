#pragma once

#include <Windows.h>
#include "logger.h"
#include "GetProcId.h"
#include <string> //using stoi

class Injector
{
private:
	LPTSTR dllPath;
	DWORD processId;

	HANDLE h_process;
	LPVOID allocated_memory;

public:
	Injector();
	~Injector();

	BOOL inject();
	BOOL eject();
	BOOL setDllPath(LPTSTR);
	BOOL setProcessId(LPTSTR, LPTSTR);
};
