#include <Windows.h>
#include <conio.h>
#include <time.h>
#include "Child.h"
#define ever ;;

Child::Child() {};

Child::Child(char* argv[])
{
	run(argv);
}

Child::~Child() {};

void Child::run(char* argv[]) 
{
	char bufID[10];

	sprintf_s(bufID, "%dprint", atoi(argv[1]));
	HANDLE print = OpenEvent(EVENT_ALL_ACCESS, FALSE, bufID);

	sprintf_s(bufID, "%dclose", atoi(argv[1]));
	HANDLE close = OpenEvent(EVENT_ALL_ACCESS, FALSE, bufID);

	sprintf_s(bufID, "%dstop", atoi(argv[1]));
	HANDLE stop = OpenEvent(EVENT_ALL_ACCESS, FALSE, bufID);

	char outputRow[50];
	sprintf_s(outputRow, "som%d", atoi(argv[1])); 	//WE WILL FILL THIS FURTHER

	for(ever) {

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

	}
}