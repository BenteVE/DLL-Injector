#include "parser.h"
#include "injector.h"

int main()
{

	Injector injector;

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