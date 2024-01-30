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

static LPTSTR ReadINI(LPCTSTR lpAppName, LPCTSTR lpKeyName)
{
	LPCTSTR lpFileName = TEXT(".\\conf.ini");
	DWORD nSize = 255;

	LPTSTR lpReturnedString = new TCHAR[nSize];

	GetPrivateProfileString(lpAppName, lpKeyName, NULL, lpReturnedString, nSize, lpFileName);

	return lpReturnedString;
}

static void log_error(LPCTSTR error) {
	tcout << TEXT("Error: ") << error << std::endl;
}

static void log(LPCTSTR message) {
	tcout << TEXT("Log: ") << message << std::endl;
}

int main()
{
	LPTSTR dll = ReadINI(TEXT("DLL"), TEXT("PATH"));
	LPTSTR dllPath = new TCHAR[MAX_PATH];
	if (!GetFullPathName(dll, MAX_PATH, dllPath, nullptr))
	{
		log_error(TEXT("DLL not found"));
		system("pause");
		exit(1);
	}
	else {
		log(TEXT("Found DLL"));
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
			log_error(TEXT("Invalid Process ID in conf.ini"));
			system("pause");
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
		log_error(TEXT("invalid mode in conf.ini"));
		system("pause");
		exit(1);
	}

	delete[] mode;

	log(TEXT("Acquired process ID"));

	//open process with read and write permissions
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
	if (!h_process)
	{
		log_error(TEXT("OpenProcess failed to acquire a handle to the process"));
		system("pause");
		exit(1);
	}
	log(TEXT("OpenProcess acquired the process handle"));

	//allocate memory in external process for path name (we don't need execute permissions)
	void* allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!allocated_memory)
	{
		log_error(TEXT("VirtualAllocEx failed to allocate memory in target application"));
		system("pause");
		exit(1);
	}
	log(TEXT("VirtualAllocEx allocated memory in target application"));

	//write path in process memory
	if (!WriteProcessMemory(h_process, allocated_memory, dllPath, MAX_PATH, nullptr))
	{
		log_error(TEXT("WriteProcessMemory failed to write process memory"));
		system("pause");
		exit(1);
	}
	log(TEXT("WriteProcessMemory wrote DLL path in memory of target application"));

	// create remote thread in target process that will call LoadLibrary (loc = address with path to DLL)
	HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibrary), allocated_memory, NULL, nullptr);
	if (!h_thread)
	{
		log_error(TEXT("CreateRemoteThread failed to create remote thread in target application"));
		system("pause");
		exit(1);
	}
	log(TEXT("CreateRemoteThread created remote thread in target application to run LoadLibrary and load the DLL"));

	// dll is loaded, program done, close handle
	VirtualFreeEx(h_process, allocated_memory, NULL, MEM_RELEASE);
	CloseHandle(h_process);

	delete[] dll;

	log(TEXT("DLL injection successful"));
	system("pause");
	exit(0);
}