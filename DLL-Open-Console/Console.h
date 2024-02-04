#pragma once

#include <Windows.h>
#include <iostream>

class Console {
private:
	// Pointer to the opened stream
	FILE* stream;

public:
	Console();
	~Console();

	BOOL open();
	void log(const char*);
};
