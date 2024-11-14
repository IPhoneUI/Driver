#include <iostream>

int main()
{
    std::cout << "Driver is initialized" << std::endl;
    int a = 0;
    while (true) {
        ++a;
        if (a == 1000000000)
        {
            std::cout << "Driver is runninggggg" << std::endl;
            a = 0;
        }
    }
}