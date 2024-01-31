#pragma once

#include <Windows.h>
#include "logger.h"
#include "GetProcId.h"

class Injector {
private:
	LPCTSTR dllName;
	LPCTSTR dllPath;
	DWORD processId;

	HANDLE h_process;
	LPVOID allocated_memory;

public:
	Injector(LPTSTR, LPTSTR, DWORD);
	~Injector();

	BOOL inject();
	BOOL eject();
};
