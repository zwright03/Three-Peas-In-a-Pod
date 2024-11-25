#include "timer.h"
#include <iostream>

int main() {
	std::cout << "Hello World! 1" << std::endl;
	
	{
		Timer timer("Test"); // When the timer is initialized it starts
		int j = 0;
		for (int i = 0; i < 1000000; i++)
			j += i;
	} // When the timer falls out of scope and is destroyed, it will print out the time it took.

	std::cout << "Hello World! 2" << std::endl;
	return 0;
}