#pragma once
#include <vector>

struct DataPoint {
	bool vendorID; // 0 for CMT, 1 for Verifone (col 1 of csv)
	short ratecodeID; // (1 = Standard rate; 2 = JFK; 3 = Newark; 4 = Nassau or Westchester; 5 = Negotiated fare; 6 = Group ride) (col 5 of csv)
	short passenger_count; // (col 8 of csv)
	float trip_distance; // in miles (col 9 of csv)
	float total_amount; // (col 16 of csv)

	DataPoint() = default;
	DataPoint(bool vendorID, short ratecode, short passenger_count, float trip_distance, float total_amount);
};

// returns a vector of the data in the data file.
std::vector<DataPoint> loadData();

// merge sort to be timed and have it's performance measured
void mergeSort(std::vector<DataPoint>& data, int left, int right);

// quick sort, to be timed and have it's performance measured
void quickSort(std::vector<DataPoint>& data, int low, int high);
