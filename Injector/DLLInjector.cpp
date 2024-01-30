#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

#include <fstream>

#include "GetProcId.h"

LPWSTR ReadINI(LPCWSTR lpAppName, LPCWSTR lpKeyName)
{
	LPCTSTR lpFileName = L".\\conf.ini";
	DWORD nSize = 255;

	LPWSTR lpReturnedString = new WCHAR[nSize];

	GetPrivateProfileString(lpAppName, lpKeyName, NULL, lpReturnedString, nSize, lpFileName);

	return lpReturnedString;
}

int main()
{
	// get the DLL 
	LPWSTR dll = ReadINI(L"DLL", L"PATH");
	LPWSTR dllPath = new WCHAR[MAX_PATH];
	if (!GetFullPathName(dll, MAX_PATH, dllPath, nullptr))
	{
		std::wcout << L"Configuration Error: DLL not found" << std::endl;
		exit(1);
	}

	LPWSTR mode = ReadINI(L"TARGET", L"MODE");
	DWORD processId = NULL;

	if (lstrcmpW(mode, L"PROCESS_ID") == 0)
	{
		LPWSTR process_id = ReadINI(L"TARGET", mode);
		try
		{
			processId = std::stoi(process_id);
		}
		catch (const std::exception&)
		{
			std::wcout << L"Configuration Error: Invalid Process ID" << std::endl;
			exit(1);
		}
		delete[] process_id;
	}
	else if (lstrcmpW(mode, L"PROCESS_NAME") == 0) {
		LPWSTR process_name = ReadINI(L"TARGET", mode);
		processId = GetProcId(process_name);
		delete[] process_name;
	}
	else if (lstrcmpW(mode, L"WINDOW_TITLE") == 0) {
		LPWSTR window_title = ReadINI(L"TARGET", mode);
		GetProcIdWindowTitle(window_title, processId);
		delete[] window_title;
	}
	else {
		std::wcout << L"Configuration Error: Target settings invalid" << std::endl;
		exit(1);
	}

	delete[] mode;

	std::wcout << L"Acquired process ID" << std::endl;

	//open process with read and write permissions
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
	if (!h_process)
	{
		std::wcout << L"OpenProcess Error: Failed to open a handle to process" << std::endl;
		exit(1);
	}
	std::wcout << L"OpenProcess: Acquired process handle" << std::endl;

	//allocate memory in external process for path name (we don't need execute permissions)
	void* allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!allocated_memory)
	{
		std::wcout << L"VirtualAllocEx Error: Failed to allocate memory" << std::endl;
		exit(1);
	}
	std::wcout << L"VirtualAllocEx: Allocated memory in target application" << std::endl;

	//write path in process memory
	if (!WriteProcessMemory(h_process, allocated_memory, dllPath, MAX_PATH, nullptr))
	{
		std::wcout << L"WriteProcessMemory Error: Failed to write process memory" << std::endl;
		exit(1);
	}
	std::wcout << L"WriteProcessMemory: DLL path written in memory of target application" << std::endl;

	// create remote thread in target process that will call LoadLibrary (loc = address with path to DLL)
	HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibrary), allocated_memory, NULL, nullptr);
	if (!h_thread)
	{
		std::wcout << L"CreateRemoteThread Error:Failed to create remote thread" << std::endl;
		exit(1);
	}
	std::wcout << L"CreateRemoteThread: Created remote thread in target application to run LoadLibrary and load the DLL" << std::endl;

	// dll is loaded, program done, close handle
	VirtualFreeEx(h_process, allocated_memory, NULL, MEM_RELEASE);
	CloseHandle(h_process);

	delete[] dll;

	std::wcout << L"Successfully injected" << std::endl;
	system("pause");
	exit(0);
}