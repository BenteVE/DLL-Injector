#include "parser.h"

static const LPCTSTR CONFIG_FILE = TEXT(".\\conf.ini");

BOOL getDll(LPTSTR dll, LPTSTR dllPath, DWORD szBuffer) {
	GetPrivateProfileString(TEXT("DLL"), TEXT("PATH"), NULL, dll, szBuffer, CONFIG_FILE);

	if (!GetFullPathName(dll, szBuffer, dllPath, nullptr))
	{
		log_error(TEXT("DLL not found"));
		return FALSE;
	}

	log(TEXT("Found DLL"));
	return TRUE;
}

DWORD getTarget() {
	LPTSTR mode = new TCHAR[16];
	GetPrivateProfileString(TEXT("TARGET"), TEXT("MODE"), NULL, mode, 16, CONFIG_FILE);

	DWORD processId = NULL;

	if (lstrcmp(mode, TEXT("PROCESS_ID")) == 0)
	{
		LPTSTR process_id = new TCHAR[8];
		GetPrivateProfileString(TEXT("TARGET"), mode, NULL, process_id, 8, CONFIG_FILE);
		try
		{
			processId = std::stoi(process_id);
		}
		catch (const std::exception&)
		{
			log_error(TEXT("Invalid Process ID in conf.ini"));
			return FALSE;
		}
		delete[] process_id;
	}
	else if (lstrcmp(mode, TEXT("PROCESS_NAME")) == 0)
	{
		LPTSTR process_name = new TCHAR[64];
		GetPrivateProfileString(TEXT("TARGET"), mode, NULL, process_name, 64, CONFIG_FILE);
		processId = GetProcId(process_name);
		delete[] process_name;
	}
	else if (lstrcmp(mode, TEXT("WINDOW_TITLE")) == 0)
	{
		LPTSTR window_title = new TCHAR[64];
		GetPrivateProfileString(TEXT("TARGET"), mode, NULL, window_title, 64, CONFIG_FILE);
		processId = GetProcIdWindowTitle(window_title);
		delete[] window_title;
	}
	else
	{
		log_error(TEXT("invalid mode in conf.ini"));
		return FALSE;
	}

	delete[] mode;

	log(TEXT("Acquired process ID"));
	return processId;

}