#pragma once
#include <Windows.h>
#include "logger.h"
#include "GetProcId.h"
#include <string>

// undefine macros that cause errors in argparser
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include "cxxopts.hpp"