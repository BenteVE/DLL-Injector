#include "logger.h"

void log(LPCTSTR message)
{
	tcout << TEXT("Log: ") << message << std::endl;
}

void log_error(LPCTSTR error)
{
	tcout << TEXT("Error: ") << error << std::endl;
}