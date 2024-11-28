#include "DataPoint.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
// #include <filesystem>


std::vector<DataPoint> loadData() {
	std::ifstream file("resources/data/2020_taxi_data.csv");

	// to find out where your binary is built if you cant find it:
	// put the data folder in the same place as your binary 
	// std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

	if (!file.is_open())
		throw std::runtime_error("Missing taxi information");

	std::string line;
	getline(file, line); // get the first line, which is just the column titles.

	std::vector<DataPoint> data_points;
	data_points.reserve(199999);

	while (getline(file, line)) {
		std::stringstream ss(line);

		std::string buffer;
		getline(ss, buffer, ',');

		DataPoint data;
		data.vendorID = (stoi(buffer) - 1);

		for (int i = 0; i < 3; i++) 
			getline(ss, buffer, ',');
		

		getline(ss, buffer, ',');
		data.ratecodeID = stoi(buffer);

		for (int i = 0; i < 2; i++) 
			getline(ss, buffer, ',');
		

		getline(ss, buffer, ',');
		data.passenger_count = stoi(buffer);

		getline(ss, buffer, ',');
		data.trip_distance = stof(buffer);

		for (int i = 0; i < 6; i++) 
			getline(ss, buffer, ',');
		

		getline(ss, buffer, ',');
		data.total_amount = stof(buffer);

		data_points.push_back(data);
	}

	file.close();
	return data_points;
}

DataPoint::DataPoint(bool vendorID, short ratecode, short passenger_count, float trip_distance, float total_amount)
	: vendorID(vendorID), ratecodeID(ratecode), passenger_count(passenger_count), 
	trip_distance(trip_distance), total_amount(total_amount) {}
