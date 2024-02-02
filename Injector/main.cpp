#include "parser.h"
#include "injector.h"
#include "logger.h"

#ifdef UNICODE
#define tmain wmain
#else
#define tmain main
#endif

int tmain(int argc, TCHAR* argv[])
{
	for (int i = 1; i < argc; i++) {
		log(argv[i]);
	}

	DWORD processId = 0;

	if (processId == 0) {
		system("pause");
		exit(1);
	}

	Injector injector(NULL, processId);

	if (!injector.initialize()) {
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