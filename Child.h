#pragma once
#include <iostream>

class Child
{

public:
	Child();
	Child(char* argv[]);
	~Child();

	void run(char* argv[]);
};