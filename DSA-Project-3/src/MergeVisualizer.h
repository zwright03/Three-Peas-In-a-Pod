#pragma once
#include "SortVisualizer.h"
#include <vector>

class MergeVisualizer : public Visualizer {
private:
    int left, right;
    bool initialized;
    std::vector<int> indices;
    void merge(std::array<SortShape, 50>& shapes, int left, int mid, int right);
    void mergeSort(std::array<SortShape, 50>& shapes, int left, int right);

public:
    MergeVisualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case = false);
    void iterate() override;
};


