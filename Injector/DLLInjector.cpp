#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

#include <fstream>

#include "GetProcId.h"

char* ReadINI(const char* szSection, const char* szKey, const char* szDefaultValue)
{
	char* szResult = new char[255];
	memset(szResult, 0x00, 255);
	GetPrivateProfileString(szSection, szKey,
		szDefaultValue, szResult, 255, ".\\conf.ini");
	return szResult;
}

int main()
{
	// check if the settings file exists
	if (false)
	{
		MessageBox(NULL, "settings file conf.ini not found", "settings file not found", NULL);
		exit(-1);
	}

	// check if dll exists
	char* dll = ReadINI("DLL", "PATH", NULL);
	char dllPath[MAX_PATH];
	if (!GetFullPathName(dll, MAX_PATH, dllPath, nullptr))
	{
		MessageBox(NULL, "DLL not found", "DLL not found", NULL);
		exit(-1);
	}

	char* mode = ReadINI("TARGET", "MODE", NULL);
	DWORD processId = NULL;

	if (strcmp(mode, "PROCESS_ID") == 0)
	{
		char* process_id = ReadINI("TARGET", mode, NULL);
		try
		{
			processId = std::atoi(process_id);
		}
		catch (const std::exception&)
		{
			MessageBox(NULL, "Invalid Process ID", "Configuration Error", NULL);
			exit(-1);
		}
	}
	else if (strcmp(mode, "PROCESS_NAME") == 0) {
		char* process_name = ReadINI("TARGET", mode, NULL);
		processId = GetProcId(process_name);
	}
	else if (strcmp(mode, "WINDOW_TITLE") == 0) {
		char* window_title = ReadINI("TARGET", mode, NULL);
		GetProcIdWindowTitle(window_title, processId);
	}
	else {
		MessageBox(NULL, "Target settings invalid", "Configuration Error", NULL);
		exit(-1);
	}

	//open process with read and write permissions
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
	if (!h_process)
	{
		MessageBox(NULL, "Failed to open a handle to process", "OpenProcess", NULL);
		exit(-1);
	}

	//allocate memory in external process for path name (we don't need execute permissions)
	void* allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!allocated_memory)
	{
		MessageBox(NULL, "VirtualAllocEx", "Failed to allocate memory", NULL);
		exit(-1);
	}

	//write path in process memory
	if (!WriteProcessMemory(h_process, allocated_memory, dllPath, MAX_PATH, nullptr)) //unicode injector: Turns out WriteProcessMemory wanted DLL_NAME to be a char. converted wchar_t* to char in a new variable and now i have no problems.
	{
		MessageBox(NULL, "WriteProcessMemory", "Failed to write process memory", NULL);
		exit(-1);
	}

	// create remote thread in target process that will call LoadLibraryA (loc = address with path to DLL)
	HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocated_memory, NULL, nullptr);
	if (!h_thread)
	{
		MessageBox(NULL, "CreateRemoteThread", "Failed to create remote thread", NULL);
		exit(-1);
	}

	// dll is loaded, program done, close handle
	CloseHandle(h_process);
	VirtualFreeEx(h_process, allocated_memory, NULL, MEM_RELEASE);
	MessageBox(0, "Successfully injected", "Success", 0);
}