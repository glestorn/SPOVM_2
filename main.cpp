#include<iostream>

#include "Parent.h"
#include "Child.h"

int main(int argc, char* argv[]) 
{
    try {
        if (argc <= 1) {
            Parent mainBody;
        }
        else {
#ifdef __linux__
            Child request(argv[0], argv[1]);
#elif   _WIN32
			Child request(argv[1]);
#endif
        }
    }
    catch (...) {
        return 1;
    }

    return 0;
}