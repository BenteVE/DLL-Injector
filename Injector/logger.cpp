#include "logger.h"

void log(LPCTSTR message)
{
	tcout << TEXT("Log: ") << message << std::endl;
}

void log_error(LPCTSTR error)
{
	tcout << TEXT("Error: ") << error << std::endl;
}

void displayHelp()
{
	tcout
		<< TEXT("Usage: ./DLLInjector.exe <dll_path> <identification_method> <target_argument>\n")
		<< TEXT("\nArguments:\n")
		<< TEXT("  <dll_path>:      Path to the .dll file to be injected.\n")
		<< TEXT("  <identification_method>:   Method to identify the target application.\n")
		<< TEXT("                             Options: --process-id, --process-name, --window-title.\n")
		<< TEXT("  <target_argument>: Argument based on the identification method.\n")
		<< TEXT("                     If --process-id, provide an integer.\n")
		<< TEXT("                     If --process-name or --window-title, provide a string.\n")
		<< std::endl;
}