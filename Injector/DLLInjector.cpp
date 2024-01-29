#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

#include <fstream>

#include "ReadFile.h"
#include "Error.h"
#include "GetProcId.h"

int main()
{
    
    // read data from settings file
    const char* fileName = "DLLInjectorSettings.txt";

    std::string dllName;
    std::string programId; // windowTitle or processName
    std::string program;
    ReadSettingsFile(fileName, dllName, programId, program);

    if (!FileExists(dllName))
    {
        DisplayError("fileExists", "Dll file doesn't exist");
    }

    char dllPath[MAX_PATH];
    if (!GetFullPathName(dllName.c_str(), MAX_PATH, dllPath, nullptr))
    {
        DisplayError("GetFullPathName", "Failed to get full path");
    }

    DWORD processId = NULL;

    if (programId=="windowTitle")
    {
        GetProcIdWindowTitle(program.c_str(), processId);
    }
    else if (programId=="processName")
    {
        processId = GetProcId(program.c_str());
    }
    else
    {
        DisplayError("Settings file", "Wrong programId");
    }

    if (processId == NULL)
    {
        DisplayError("processId", "Failed to get process Id");
    }

    //open process with read and write permissions
    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
    if (!h_process)
    {
        DisplayError("OpenProcess", "Failed to open a handle to process");
    }

    //allocate memory in external process for path name (we don't need execute permissions)
    void* allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!allocated_memory)
    {
        DisplayError("VirtualAllocEx", "Failed to allocate memory");
    }

    //write path in process memory
    if (!WriteProcessMemory(h_process, allocated_memory, dllPath, MAX_PATH, nullptr)) //unicode injector: Turns out WriteProcessMemory wanted DLL_NAME to be a char. converted wchar_t* to char in a new variable and now i have no problems.
    {
        DisplayError("WriteProcessMemory", "Failed to write process memory");
    }

    // create remote thread in target process that will call LoadLibraryA (loc = address with path to DLL)
    HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocated_memory, NULL, nullptr);
    if (!h_thread)
    {
        DisplayError("CreateRemoteThread", "Failed to create remote thread");
    }

    // dll is loaded, program done, close handle
    CloseHandle(h_process);
    VirtualFreeEx(h_process, allocated_memory, NULL, MEM_RELEASE);
    MessageBox(0, "Successfully injected", "Success", 0);
}