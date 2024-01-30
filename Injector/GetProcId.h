#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcId(LPCWSTR processName);

void GetProcIdWindowTitle(LPCWSTR windowTitle, DWORD& processId);
