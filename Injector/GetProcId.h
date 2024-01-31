#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcId(LPCTSTR processName);
DWORD GetProcIdWindowTitle(LPCTSTR windowTitle);
HMODULE getLoadedModule(DWORD process_id, LPCTSTR dll);
