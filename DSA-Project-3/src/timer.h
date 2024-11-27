#pragma once
#include <chrono>
#include <iostream>

class Timer {
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;
	std::string fn_name;

public: 
	Timer(); // timer starts when object is created
	Timer(std::string Function_Name);

	~Timer(); // timer stops when it falls out of scope or gets deleted
};