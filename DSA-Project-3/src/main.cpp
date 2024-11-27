#include "DataPoint.h"
#include "timer.h"
#include <iostream>

int main() {

	{
		Timer timer;
		auto data_points = loadData();
	}

	std::cin.get();

	return 0;
}