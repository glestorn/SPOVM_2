#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>

class Parent
{
	std::vector<PROCESS_INFORMATION> procInfo;
	std::vector<HANDLE> closeEventInfo;
	std::vector<HANDLE> stopEventInfo;
	std::vector<HANDLE> printEventInfo;

public:
	Parent();
	~Parent();

	void run();
	void addProcess(int nextEventID);
	PROCESS_INFORMATION createProcess(char* nextEventID);
	void removeProcess();
};