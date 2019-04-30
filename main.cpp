#include <string>
#include<iostream>

#include "Parent.h"
#include "Child.h"

int main(int argc, char* argv[]) 
{
    try {
        if (argc <= 1) {
            Parent tmp;
        }
        else {
            //std::cout << "Incorrect inp" << std::endl;
#ifdef __linux__
            Pharmacy request(argv[0],argv[1]);
#elif   _WIN32 | _WIN64
			Child request(argv);
#endif
        }
    }
    catch (...) {
        return 1;
    }

    return 0;
}