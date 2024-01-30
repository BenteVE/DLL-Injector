#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcId(LPCTSTR processName);

void GetProcIdWindowTitle(LPCTSTR windowTitle, DWORD& processId);
