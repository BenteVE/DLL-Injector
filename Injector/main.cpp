#include "injector.h"
#include "logger.h"

#include <tchar.h> // using _tmain macro

int _tmain(int argc, TCHAR* argv[])
{

	if (argc == 2 && argv[1] == TEXT("--help")) {
		displayHelp();
		return 0;
	}
	else if (argc != 4) {
		displayHelp();
		return 1;
	}

	Injector injector;	

	if (!injector.setDllPath(argv[1]) || !injector.setProcessId(argv[2], argv[3])) {
		system("pause");
		exit(1);
	}

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
}