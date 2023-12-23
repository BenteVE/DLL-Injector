#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcId(const char* processName);

void GetProcIdWindowTitle(const char* windowTitle, DWORD& processId);
