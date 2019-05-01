#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#include <time.h>

#elif __linux__
#include <unistd.h>
#include <signal.h>
#include <ncurses.h>
#include <cstring>
#endif

#include "Child.h"
#define ever ;;

#ifdef __linux__
bool Print = false;
bool Close = false;
#endif

Child::Child() {};

Child::Child(char* argv, char* nonUse)
{
	run(argv);
}

Child::Child(char* argv)
{
	run(argv);
}

Child::~Child() {};

#ifdef __linux__
void exitProcess(int signal)
{
	Close = true;
}

void printLine(int signal)
{
	Print = true;
}
#endif

void Child::run(char* argv) 
{
	#ifdef _WIN32
	char bufID[10];

	sprintf_s(bufID, "%dprint", atoi(argv[1]));
	HANDLE print = OpenEvent(EVENT_ALL_ACCESS, FALSE, bufID);

	sprintf_s(bufID, "%dclose", atoi(argv[1]));
	HANDLE close = OpenEvent(EVENT_ALL_ACCESS, FALSE, bufID);

	sprintf_s(bufID, "%dstop", atoi(argv[1]));
	HANDLE stop = OpenEvent(EVENT_ALL_ACCESS, FALSE, bufID);

	char outputRow[50];
	sprintf_s(outputRow, "Process %d", atoi(argv[1]));


	#elif __linux__
	struct sigaction exitSignal;
	exitSignal.sa_handler = exitProcess;
	sigaction(SIGUSR1, &exitSignal, NULL);

	struct sigaction printSignal;
	printSignal.sa_handler = printLine;
	sigaction(SIGUSR2, &printSignal, NULL);

	char str[10];
	sprintf(str, "Proc%d", atoi(argv));
	#endif


	for(ever) {

		#ifdef _WIN32
		if (WaitForSingleObject(print, 1) == WAIT_OBJECT_0) {

			for (int i = 0; i < strlen(outputRow); i++) {
				printf("%c", outputRow[i]);
				Sleep(75);
			}
			std::cout << std::endl;
			ResetEvent(print);
			SetEvent(stop);
		}

		if (WaitForSingleObject(close, 1) == WAIT_OBJECT_0) {
			
			CloseHandle(close);
			CloseHandle(print);
			return;
		}


		#elif __linux__
		usleep(10000);
		if (Print) {
			Print = false;
			for(int i = 0; i < strlen(str); i++) {
				napms(350);
				printf("%c\n", str[i]);
			}
			kill(getppid(), SIGUSR1);
		}
		if (Close) {
			exit(0);
		}
		#endif
	}
}

