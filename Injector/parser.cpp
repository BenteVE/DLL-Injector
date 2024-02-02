#include "parser.h"

#include <iostream>
#include <string>

void displayHelp() {
    std::wcout << TEXT("Usage: YourProgramName <dll_path> <identification_method> <target_argument>\n")
               << TEXT("\nArguments:\n")
               << TEXT("  <dll_path>:      Path to the .dll file to be injected.\n")
               << TEXT("  <identification_method>:   Method to identify the target application.\n")
               << TEXT("                             Options: process_id, process_name, window_title.\n")
               << TEXT("  <target_argument>: Argument based on the identification method.\n")
               << TEXT("                     If process_id, provide an integer.\n")
               << TEXT("                     If process_name or window_title, provide a string.\n")
               << std::endl;
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc != 4) {
        displayHelp();
        return 1;
    }

    // Parse command-line arguments
    std::wstring dllPath(argv[1]);
    std::wstring identificationMethod(argv[2]);
    std::wstring targetArgument(argv[3]);

    // Check for help argument
    if (identificationMethod == TEXT("--help")) {
        displayHelp();
        return 0;
    }

    // Validate identification method
    if (!(identificationMethod == TEXT("process_id") || identificationMethod == TEXT("process_name") || identificationMethod == TEXT("window_title"))) {
        std::wcerr << TEXT("Invalid identification method. Options: process_id, process_name, window_title.") << std::endl;
        return 1;
    }

    // Perform action based on identification method
    if (identificationMethod == TEXT("process_id")) {
        try {
            int processId = std::stoi(targetArgument);
            // Your logic for process_id
            std::wcout << TEXT("Injecting ") << dllPath << TEXT(" into process with ID ") << processId << std::endl;
        } catch (const std::invalid_argument&) {
            std::wcerr << TEXT("Invalid process ID. Please provide a valid integer.") << std::endl;
            return 1;
        } catch (const std::out_of_range&) {
            std::wcerr << TEXT("Process ID out of range. Please provide a valid integer.") << std::endl;
            return 1;
        }
    } else {
        // Your logic for process_name or window_title
        std::wcout << TEXT("Injecting ") << dllPath << TEXT(" using ") << identificationMethod << TEXT(" (") << targetArgument << TEXT(")") << std::endl;
    }

    return 0;
}
