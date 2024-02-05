#pragma once

#include <Windows.h>
#include <iostream>

class Console {
public:
	// Pointer to the opened stream
	FILE* stream;

	Console();
	~Console();

	BOOL open();
};
