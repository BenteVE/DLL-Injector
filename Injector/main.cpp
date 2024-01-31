#include "parser.h"
#include "injector.h"

int main()
{
	// get values from parser
	LPTSTR dllName = new TCHAR[MAX_PATH];
	LPTSTR dllPath = new TCHAR[MAX_PATH];
	if (!getDll(dllName, dllPath, MAX_PATH)) {
		system("pause");
		exit(1);
	}

	DWORD processId = getTarget();

	Injector injector(dllName, dllPath, processId);

	// inject
	if (!injector.inject()) {
		system("pause");
		exit(1);
	}

	// wait for ejection
	log(TEXT("Press any key to unload the DLL"));
	system("pause");

	// eject
	if (!injector.eject()) {
		system("pause");
		exit(1);
	}

	system("pause");

	// clean
	delete[] dllName;
	delete[] dllPath;
}