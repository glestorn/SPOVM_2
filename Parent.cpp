//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
// #include <ctime>
#include "Parent.h"
#define ever ;;

Parent::Parent() 
{
	run();
}

Parent::~Parent() {};


void Parent::run()
{
	int currEvent = 0;
	int eventID = 1;
	char choice;
			
	std::cout << "Enter + to add a person" << std::endl;
	std::cout << "Enter - to remove a person" << std::endl;
	std::cout << "Enter q to exit" << std::endl;

	for(ever) {

		if (!printEventInfo.empty()) {
			if (currEvent == procInfo.size() - 1) {
				currEvent = -1;
			}
			SetEvent(printEventInfo[++currEvent]);
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
					currEvent = -1;
				}
				eventID--;
				removeProcess();
			}

			if (choice == 'q') {
				while(!procInfo.empty()) {
					removeProcess();
				}
				return;
			}
		}
	}
}

void Parent::addProcess(int nextEventID) 
{
	char newEventString[10];

	sprintf_s(newEventString, "%d", nextEventID);
	procInfo.push_back(createProcess(newEventString));

	sprintf_s(newEventString, "%dclose", nextEventID);
	closeEventInfo.push_back(CreateEvent(NULL, FALSE, FALSE, newEventString));

	sprintf_s(newEventString, "%dstop", nextEventID);
	stopEventInfo.push_back(CreateEvent(NULL, FALSE, FALSE, newEventString));

	sprintf_s(newEventString, "%dprint", nextEventID);
	printEventInfo.push_back(CreateEvent(NULL, FALSE, FALSE, newEventString));
}

void Parent::removeProcess()
{
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
}

PROCESS_INFORMATION Parent::createProcess(char* nextEventID) 
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof si;				//check this row in 1 lab
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