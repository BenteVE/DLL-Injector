#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

#include <fstream>

#include "GetProcId.h"

#ifdef UNICODE
#define tcout std::wcout
#else
#define tcout std::cout
#endif

LPTSTR ReadINI(LPCTSTR lpAppName, LPCTSTR lpKeyName)
{
	LPCTSTR lpFileName = TEXT(".\\conf.ini");
	DWORD nSize = 255;

	LPTSTR lpReturnedString = new TCHAR[nSize];

	GetPrivateProfileString(lpAppName, lpKeyName, NULL, lpReturnedString, nSize, lpFileName);

	return lpReturnedString;
}

int main()
{
	// get the DLL 
	LPTSTR dll = ReadINI(TEXT("DLL"), TEXT("PATH"));
	LPTSTR dllPath = new TCHAR[MAX_PATH];
	if (!GetFullPathName(dll, MAX_PATH, dllPath, nullptr))
	{
		tcout << L"Configuration Error: DLL not found" << std::endl;
		exit(1);
	}

	LPTSTR mode = ReadINI(TEXT("TARGET"), TEXT("MODE"));
	DWORD processId = NULL;

	if (lstrcmp(mode, TEXT("PROCESS_ID")) == 0)
	{
		LPTSTR process_id = ReadINI(TEXT("TARGET"), mode);
		try
		{
			processId = std::stoi(process_id);
		}
		catch (const std::exception&)
		{
			tcout << TEXT("Configuration Error: Invalid Process ID") << std::endl;
			exit(1);
		}
		delete[] process_id;
	}
	else if (lstrcmp(mode, TEXT("PROCESS_NAME")) == 0) {
		LPTSTR process_name = ReadINI(TEXT("TARGET"), mode);
		processId = GetProcId(process_name);
		delete[] process_name;
	}
	else if (lstrcmp(mode, TEXT("WINDOW_TITLE")) == 0) {
		LPTSTR window_title = ReadINI(TEXT("TARGET"), mode);
		GetProcIdWindowTitle(window_title, processId);
		delete[] window_title;
	}
	else {
		tcout << TEXT("Configuration Error: Target settings invalid") << std::endl;
		exit(1);
	}

	delete[] mode;

	tcout << TEXT("Acquired process ID") << std::endl;

	//open process with read and write permissions
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
	if (!h_process)
	{
		tcout << TEXT("OpenProcess Error: Failed to open a handle to process") << std::endl;
		exit(1);
	}
	tcout << TEXT("OpenProcess: Acquired process handle") << std::endl;

	//allocate memory in external process for path name (we don't need execute permissions)
	void* allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!allocated_memory)
	{
		tcout << TEXT("VirtualAllocEx Error: Failed to allocate memory") << std::endl;
		exit(1);
	}
	tcout << TEXT("VirtualAllocEx: Allocated memory in target application") << std::endl;

	//write path in process memory
	if (!WriteProcessMemory(h_process, allocated_memory, dllPath, MAX_PATH, nullptr))
	{
		tcout << TEXT("WriteProcessMemory Error: Failed to write process memory") << std::endl;
		exit(1);
	}
	tcout << TEXT("WriteProcessMemory: DLL path written in memory of target application") << std::endl;

	// create remote thread in target process that will call LoadLibrary (loc = address with path to DLL)
	HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibrary), allocated_memory, NULL, nullptr);
	if (!h_thread)
	{
		tcout << TEXT("CreateRemoteThread Error:Failed to create remote thread") << std::endl;
		exit(1);
	}
	tcout << TEXT("CreateRemoteThread: Created remote thread in target application to run LoadLibrary and load the DLL") << std::endl;

	// dll is loaded, program done, close handle
	VirtualFreeEx(h_process, allocated_memory, NULL, MEM_RELEASE);
	CloseHandle(h_process);

	delete[] dll;

	tcout << TEXT("Successfully injected") << std::endl;
	system("pause");
	exit(0);
}