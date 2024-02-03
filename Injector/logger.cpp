#include "logger.h"

void log(LPCTSTR message)
{
	tcout << TEXT("Log: ") << message << std::endl;
}

void log_error(LPCTSTR error)
{
	tcout << TEXT("Error: ") << error << std::endl;
}

void displayHelp() {
	std::wcout
		<< TEXT("Usage: YourProgramName <dll_path> <identification_method> <target_argument>\n")
		<< TEXT("\nArguments:\n")
		<< TEXT("  <dll_path>:      Path to the .dll file to be injected.\n")
		<< TEXT("  <identification_method>:   Method to identify the target application.\n")
		<< TEXT("                             Options: process_id, process_name, window_title.\n")
		<< TEXT("  <target_argument>: Argument based on the identification method.\n")
		<< TEXT("                     If process_id, provide an integer.\n")
		<< TEXT("                     If process_name or window_title, provide a string.\n")
		<< std::endl;
}