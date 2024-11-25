#include "timer.h"

Timer::Timer()
{
	start_time = std::chrono::high_resolution_clock::now();
}

Timer::Timer(std::string Function_Name)
{
	fn_name = Function_Name;
	start_time = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	if(fn_name.empty())
		std::cout << duration.count() << "Microseconds" << std::endl;
	else
		std::cout << "\"" << fn_name << "\"" << " function took: " << duration.count() << " microseconds." << std::endl;
}
