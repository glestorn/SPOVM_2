#pragma once
#include <iostream>

class Child
{

public:
	Child();
	Child(char* argv);
	Child(char* argv, char* nonUse);
	~Child();

	void run(char* argv);
};