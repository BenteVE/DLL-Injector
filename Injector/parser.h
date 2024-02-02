#pragma once
#define NOMINMAX // excludes min and max macros in windef.h
#include <Windows.h>

#include "logger.h"
#include "GetProcId.h"
#include <string>

#include "Injector.h"

void processArguments();