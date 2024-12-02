#include "timer.h"

// Clock used to time how long it takes for the sorting to complete with each algorithm

RAII_Timer::RAII_Timer() {
	start_time = std::chrono::high_resolution_clock::now();
}

RAII_Timer::RAII_Timer(std::string Function_Name) {
	fn_name = Function_Name;
	start_time = std::chrono::high_resolution_clock::now();
}

RAII_Timer::~RAII_Timer() {
	end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	if(fn_name.empty())
		std::cout << "Timed function took: " << duration.count() << " microseconds." << std::endl;
	else
		std::cout << "\"" << fn_name << "\"" << " function took: " << duration.count() << " microseconds." << std::endl;
}

void Timer::start() {
	start_time = std::chrono::high_resolution_clock::now();
}

int Timer::stop() {
	auto stop_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
	return duration.count();
}

int timeSortFunction(void(*sort_fn)(std::vector<DataPoint>&, int, int), std::vector<DataPoint>& data) {
	// Make the timer
	Timer timer;
	timer.start();

	// Call the sort function on the data
	sort_fn(data, 0, data.size() - 1);

	// Return the time it took to sort
	return timer.stop();
}
