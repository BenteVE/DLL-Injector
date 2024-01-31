#pragma once
#include <Windows.h>
#include "logger.h"
#include "GetProcId.h"
#include <string>

BOOL getDll(LPTSTR dll, LPTSTR dllPath, DWORD szBuffer);
DWORD getTarget();