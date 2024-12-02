#pragma once
#include <vector>

struct DataPoint {
	bool vendorID; // 0 for CMT, 1 for Verifone (col 1 of CSV)
	short ratecodeID; // (1 = Standard rate; 2 = JFK; 3 = Newark; 4 = Nassau or Westchester; 5 = Negotiated fare; 6 = Group ride) (col 5 of CSV)
	short passenger_count; // (col 8 of CSV)
	float trip_distance; // in miles (col 9 of CSV)
	float total_amount; // (col 16 of CSV)
    // Total_amount will be what we sort for our testing as almost all are different

	DataPoint() = default;
	DataPoint(bool vendorID, short ratecode, short passenger_count, float trip_distance, float total_amount);
};

// Returns a vector of the data in the data file
std::vector<DataPoint> loadData();

// Merge sort to be timed and have its performance measured
void mergeSort(std::vector<DataPoint>& data, int left, int right);

// Quick sort, to be timed and have its performance measured
void quickSort(std::vector<DataPoint>& data, int low, int high);
