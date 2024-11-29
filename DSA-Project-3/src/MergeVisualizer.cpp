#include "MergeVisualizer.h"
#include <iostream>
#include <vector>
using namespace std;

MergeVisualizer::MergeVisualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case)
        : Visualizer(window, font, rightSide, worst_case), left(0), right(shapes.size() - 1), initialized(false) {
    indices.push_back(left);
    indices.push_back(right);
    std::cout << "MergeVisualizer initialized.\n";
}

void MergeVisualizer::merge(std::array<SortShape, 50>& shapes, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<SortShape> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = shapes[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = shapes[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].getValue() <= R[j].getValue()) {
            shapes[k] = L[i];
            i++;
        } else {
            shapes[k] = R[j];
            j++;
        }
        k++;
        swaps++;
    }

    while (i < n1) {
        shapes[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        shapes[k] = R[j];
        j++;
        k++;
    }

    std::cout << "Merged [" << left << ", " << mid << "] and [" << mid + 1 << ", " << right << "]\n";
}

void MergeVisualizer::mergeSort(std::array<SortShape, 50>& shapes, int left, int right) {
    if (!std::is_sorted(shapes.begin(), shapes.end(), [](const SortShape &a, const SortShape &b) {
        return a.getValue() < b.getValue();
    })) {
        std::cout << "ERROR: Shapes array is not sorted correctly!" << std::endl;
        done = false;
    } else {
        std::cout << "SUCCESS: Shapes array is sorted correctly!" << std::endl;
        done = true;
    }

    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(shapes, left, mid);
    mergeSort(shapes, mid + 1, right);
    merge(shapes, left, mid, right);
}

void MergeVisualizer::iterate() {
    mergeSort(shapes, 0, shapes.size() - 1);
}