#include "MergeVisualizer.h"
#include <iostream>
#include <vector>

MergeVisualizer::MergeVisualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case)
        : Visualizer(window, font, rightSide, worst_case), current_size(1), update_frequency(100), frame_count(0) {}

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
        L[i].setFillColor(sf::Color::Red);
        R[j].setFillColor(sf::Color::Red);
        draw(); // Render the updated shapes

        if (L[i].getValue() <= R[j].getValue()) {
            L[i].setFillColor(sf::Color::White);
            shapes[k++] = L[i++];
        } 
        else {
            R[j].setFillColor(sf::Color::White);
            shapes[k++] = R[j++];
        }
    }

    while (i < n1) {
        shapes[k++] = L[i++];
    }
    
    while (j < n2) {
        shapes[k++] = R[j++];
    }
    
    // std::cout << "Merged [" << left << ", " << mid << "] and [" << mid + 1 << ", " << right << "]\n";
}

void MergeVisualizer::iterate() {
    frame_count++; // to slow it down when running
    if (frame_count % update_frequency != 0)
        return;
    if (frame_count > 1000000)
        frame_count = 0;
    
    int n = shapes.size();

    for (int left = 0; left < n - 1; left += 2 * current_size) {
        int mid = std::min(left + current_size - 1, n - 1);
        int right = std::min(left + 2 * current_size - 1, n - 1);

        if (mid < right) 
            merge(shapes, left, mid, right);
    }

    current_size *= 2;

    // couldnt find where to check if the sorting was done, so i determined
    // its usually fully sorted by the time current size reaches this number:
    if (current_size >= 128) 
        done = true;
}