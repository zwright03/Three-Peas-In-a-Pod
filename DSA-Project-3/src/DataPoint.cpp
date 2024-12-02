#include "DataPoint.h"
#include "timer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>


std::vector<DataPoint> loadData() {
	std::ifstream file("resources/data/2020_taxi_data.csv");

	if (!file.is_open())
		throw std::runtime_error("Missing taxi information");

	std::string line;
	getline(file, line); // get the first line, which is just the column titles.

	std::vector<DataPoint> data_points;
	data_points.reserve(125000);

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

    std::cout << data_points.size() << " entries were loaded from the CSV file" << std::endl;

    return data_points;
}

DataPoint::DataPoint(bool vendorID, short ratecode, short passenger_count, float trip_distance, float total_amount)
	: vendorID(vendorID), ratecodeID(ratecode), passenger_count(passenger_count),
	trip_distance(trip_distance), total_amount(total_amount) {}


static void merge(std::vector<DataPoint>& data, int left, int mid, int right)
{
	int i, j, k;
	int n1 = mid - left + 1;
	int n2 = right - mid;

	// temp vectors
	std::vector<DataPoint> L(n1), R(n2);

	// copy data into temp vectors
	for (i = 0; i < n1; i++)
		L[i] = data[left + i];
	for (j = 0; j < n2; j++)
		R[j] = data[mid + 1 + j];

	// merge them back into data vector
	i = 0;
	j = 0;
	k = left;
	while (i < n1 && j < n2) {
		if (L[i].total_amount <= R[j].total_amount) {
			data[k] = L[i];
			i++;
		}
		else {
			data[k] = R[j];
			j++;
		}
		k++;
	}

	// copy the remaining things in L
	while (i < n1) {
		data[k] = L[i];
		i++;
		k++;
	}

	// copy the remaining things in R
	while (j < n2) {
		data[k] = R[j];
		j++;
		k++;
	}
}

void mergeSort(std::vector<DataPoint>& data, int left, int right)
{
	if (left < right) {
		int mid = left + (right - left) / 2;

		// sort both halves
		mergeSort(data, left, mid);
		mergeSort(data, mid + 1, right);

		// merge the halves
		merge(data, left, mid, right);
	}
}

static int partition(std::vector<DataPoint>& data, int low, int high)
{
    int mid = low + (high - low) / 2;

    if (data[low].total_amount > data[mid].total_amount)
        std::swap(data[low], data[mid]);
    if (data[low].total_amount > data[high].total_amount)
        std::swap(data[low], data[high]);
    if (data[mid].total_amount > data[high].total_amount)
        std::swap(data[mid], data[high]);

    std::swap(data[mid], data[high]);

    DataPoint pivot = data[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (data[j].total_amount <= pivot.total_amount) {
            i++;
            std::swap(data[i], data[j]);
        }
    }

    std::swap(data[i + 1], data[high]);

    return (i + 1);
}

void quickSort(std::vector<DataPoint>& data, int low, int high)
{
	if (low < high) {
		int partition_idx = partition(data, low, high);

		quickSort(data, low, partition_idx - 1);
		quickSort(data, partition_idx + 1, high);
	}
}
// go back to this