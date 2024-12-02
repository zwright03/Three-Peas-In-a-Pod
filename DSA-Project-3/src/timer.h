#pragma once
#include "DataPoint.h"
#include <chrono>
#include <iostream>
#include <vector>

class RAII_Timer {
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;
	std::string fn_name;

public: 
	RAII_Timer(); // Timer starts when object is created
	RAII_Timer(std::string Function_Name);
	~RAII_Timer(); // Timer stops when it falls out of scope or gets deleted
};

class Timer { // Timer that you can call start and stop on
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

public:
	Timer() = default;
	void start();
	int stop(); // Returns the time in microseconds that the timer took
};

// Functions for measuring performance of the sets, will return the time it took to sort
int timeSortFunction(void (*sort_fn)(std::vector<DataPoint>&, int, int), std::vector<DataPoint>& data);
