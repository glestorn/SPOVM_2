#pragma once

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>

#elif __linux__
#include <signal.h>
#endif

class Parent
{
	#ifdef _WIN32
	std::vector<PROCESS_INFORMATION> procInfo;
	std::vector<HANDLE> closeEventInfo;
	std::vector<HANDLE> stopEventInfo;
	std::vector<HANDLE> printEventInfo;
	#endif

public:
	Parent();
	~Parent();

	void run();
	void addProcess(int nextEventID);
	void removeProcess();
	
	#ifdef _WIN32	
	PROCESS_INFORMATION createProcess(char* nextEventID);
	#endif
};