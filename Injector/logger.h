#pragma once
#include <Windows.h>
#include <iostream>

#ifdef UNICODE
#define tcout std::wcout
#else
#define tcout std::cout
#endif

void log(LPCTSTR message);
void log_error(LPCTSTR error);
