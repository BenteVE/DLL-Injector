#include "Injector.h"

Injector::Injector(LPTSTR dllName, LPTSTR dllPath, DWORD processId) {
	this->dllName = dllName;
	this->dllPath = dllPath;
	this->processId = processId;
	
	h_process = 0;
	allocated_memory = nullptr;
}

Injector::~Injector() {
	VirtualFreeEx(h_process, allocated_memory, NULL, MEM_RELEASE);
	CloseHandle(h_process);
}

BOOL Injector::inject() {
	// open process with read and write permissions
	h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
	if (!h_process)
	{
		log_error(TEXT("OpenProcess failed to acquire a handle to the process"));
		return FALSE;
	}
	log(TEXT("OpenProcess acquired the process handle"));

	// allocate memory in external process for path name (we don't need execute permissions)
	allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!allocated_memory)
	{
		log_error(TEXT("VirtualAllocEx failed to allocate memory in target application"));
		return FALSE;
	}
	log(TEXT("VirtualAllocEx allocated memory in target application"));

	// write path in process memory
	if (!WriteProcessMemory(h_process, allocated_memory, dllPath, MAX_PATH, nullptr))
	{
		log_error(TEXT("WriteProcessMemory failed to write process memory"));
		return FALSE;
	}
	log(TEXT("WriteProcessMemory wrote DLL path in memory of target application"));

	// create remote thread in target process that will call LoadLibrary (loc = address with path to DLL)
	HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibrary), allocated_memory, NULL, nullptr);
	if (!h_thread)
	{
		log_error(TEXT("CreateRemoteThread failed to create remote thread in target application"));
		return FALSE;
	}
	CloseHandle(h_thread);
	log(TEXT("CreateRemoteThread created remote thread in target application to run LoadLibrary and load the DLL"));
	return TRUE;
}

BOOL Injector::eject() {
	// Find the module handle of the injected DLL
	HMODULE mod = getLoadedModule(processId, dllName);

	HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(FreeLibrary), mod, NULL, nullptr);
	if (!h_thread)
	{
		log_error(TEXT("CreateRemoteThread failed to create remote thread in target application"));
		return FALSE;
	}
	CloseHandle(h_thread);
	log(TEXT("CreateRemoteThread created remote thread in target application to run FreeLibrary and unload the DLL"));
	return TRUE;
}