#include "injector.h"
#include "logger.h"

#include <tchar.h> // using _tmain macro
#include <conio.h> // using _getch

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
		return 1;
	}

	log(TEXT("Injecting..."));
	if (!injector.inject()) {
		return 1;
	}

	tcout << TEXT("Press 'Q' to quit or 'E' to eject the dll") << std::endl;
	int ch = ' ';
	while (ch != 'Q' && ch != 'E') {
		ch = _gettch();
		ch = toupper(ch);
	}

	if (ch == 'E') {
		log(TEXT("Ejecting..."));
		if (!injector.eject()) {
			return 1;
		}
	}
	else if (ch == 'Q'){
		return 0;
	}
	
}