#include "GetProcId.h"

//Get process id by looking at snapshot of all loaded processes
DWORD GetProcId(const char* processName) {
    // we want the id of the process
    DWORD processId = 0;

    // create snapshot of processes
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // check if the snapshot is good
    if (hSnap != INVALID_HANDLE_VALUE) {

        //receive each process entry from snapshot
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry)) {//get first process
            do {
                //string compare (capital insensitive)
                if (!_stricmp(procEntry.szExeFile, processName)) {
                    //break if the right process is found
                    processId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry)); //loop through processes
        }
    }
    CloseHandle(hSnap);
    return processId;
}

// Get process id by looking at window title
void GetProcIdWindowTitle(const char* windowTitle, DWORD& processId)
{
    GetWindowThreadProcessId(FindWindow(NULL, windowTitle), &processId);
}

