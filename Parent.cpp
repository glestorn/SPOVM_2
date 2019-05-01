#ifdef _WIN32
#include <conio.h>
#include <windows.h>

#elif __linux__
#include <unistd.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <termios.h>
#endif

#include "Parent.h"
#define ever ;;

int currEvent;
int eventID;

#ifdef __linux__
std::vector<pid_t> processes;
struct sigaction customSignal;
#endif

Parent::Parent() 
{
	run();
}

Parent::~Parent() {};


#ifdef __linux__
void setPrint(int signal)
{
	if (currEvent == processes.size())
		currEvent = 0;
	kill(processes[currEvent++], SIGUSR2);
}

int getRequest() {
	struct termios oldattr, newattr;
	int ch;
	tcgetattr( STDIN_FILENO, &oldattr );
	newattr = oldattr;
	newattr.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
	return ch;
}
#endif


void Parent::run()
{
	#ifdef __linux__
	customSignal.sa_handler = setPrint;
	sigaction(SIGUSR1, &customSignal, NULL);
	#endif

	currEvent = 0;
	eventID = 1;
	char choice;
			
	std::cout << "Enter + to add a person" << std::endl;
	std::cout << "Enter - to remove a person" << std::endl;
	std::cout << "Enter q to exit" << std::endl;

	for(ever) {

		#ifdef _WIN32
		if (!printEventInfo.empty()) {
			if (currEvent == procInfo.size()) {
				currEvent = 0;
			}
			SetEvent(printEventInfo[currEvent++]);
		}

		if (!procInfo.empty())
			WaitForSingleObject(stopEventInfo[currEvent], INFINITE);

		if (_kbhit()) {
			choice = _getch();
			if (choice == '+') {
				addProcess(eventID++);
			}
		
			if (choice == '-' && !procInfo.empty()) {
				if (printEventInfo.back() == printEventInfo[currEvent]) {
					currEvent = 0;
				}
				eventID--;
				removeProcess();
			}

			if (choice == 'q') {
				while (!procInfo.empty()) {
					removeProcess();
				}
				return;
			}
		}

		#elif __linux__
		switch (getRequest()) {
			case '+': {
				addProcess(eventID++);
				
				if (eventID == 2) {
					currEvent = 0;
					sleep(1);
					kill(processes.back(), SIGUSR2);
				}
				break;
			}

			case '-': {
				if (!processes.empty()) {
					eventID--;
					if (currEvent == processes.size()) {
						removeProcess();
						currEvent = 0;
					}
					else {
						removeProcess();
					}
				}
				break;
			}

			case 'q': {
				while (!processes.empty()) {
					removeProcess();
				}
				getRequest();
				exit(0);
			}
		}
		#endif
	}
}

void Parent::addProcess(int nextEventID) 
{
	#ifdef _WIN32
	char newEventString[10];

	sprintf_s(newEventString, "%d", nextEventID);
	procInfo.push_back(createProcess(newEventString));

	sprintf_s(newEventString, "%dclose", nextEventID);
	closeEventInfo.push_back(CreateEvent(NULL, FALSE, FALSE, newEventString));

	sprintf_s(newEventString, "%dstop", nextEventID);
	stopEventInfo.push_back(CreateEvent(NULL, FALSE, FALSE, newEventString));

	sprintf_s(newEventString, "%dprint", nextEventID);
	printEventInfo.push_back(CreateEvent(NULL, FALSE, FALSE, newEventString));


	#elif __linux__
	char params[11];
	sprintf(params, "%d", nextEventID);

	pid_t pid = fork();

	if (pid < 0) {
		printf("Bad attempt");
	}
	else if (pid > 0) {
		processes.push_back(pid);
	}
	else {
		if (execlp("./main", params, params, NULL) == -1) {
			std::cout << "ERROR" << std::endl;
		}
	}
	#endif
}

void Parent::removeProcess()
{
	#ifdef _WIN32
	SetEvent(closeEventInfo.back());
	WaitForSingleObject(procInfo.back().hProcess, INFINITE); 	//maybe it's uneccessary

	CloseHandle(closeEventInfo.back());
	CloseHandle(stopEventInfo.back());
	CloseHandle(printEventInfo.back());

	CloseHandle(procInfo.back().hProcess);
	CloseHandle(procInfo.back().hThread);

	closeEventInfo.pop_back();
	stopEventInfo.pop_back();
	printEventInfo.pop_back();
	procInfo.pop_back();


	#elif __linux__
	kill(processes.back(), SIGUSR1);
	processes.pop_back();
	#endif
}

#ifdef _WIN32
PROCESS_INFORMATION Parent::createProcess(char* nextEventID) 
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof si;
	ZeroMemory(&pi, sizeof(pi));

	char cmdLine[30];
	sprintf_s(cmdLine, "SPOVM_2 %s", nextEventID);

	if(!CreateProcess(NULL,
				  	  cmdLine,
				 	  NULL,
				  	  NULL,
				  	  FALSE,
				  	  0,
				  	  NULL,
				  	  NULL,
				  	  &si,
				  	  &pi)) {
		std::cout << "There is an error with creating of proccess" << std::endl;	
	}

	return pi;
}
#endif
