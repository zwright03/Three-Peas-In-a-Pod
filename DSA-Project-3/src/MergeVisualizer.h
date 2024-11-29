#ifndef DSA_PROJECT_3_MERGEVISUALIZER_H
#define DSA_PROJECT_3_MERGEVISUALIZER_H

#include "SortVisualizer.h"
#include <vector>

class MergeVisualizer : public Visualizer {
private:
    int left, right;
    bool initialized;
    std::vector<int> indices;

public:
    MergeVisualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case = false);
    void iterate() override;
    void merge(std::array<SortShape, 50>& shapes, int left, int mid, int right);
    void mergeSort(std::array<SortShape, 50>& shapes, int left, int right);
};

#endif // DSA_PROJECT_3_MERGEVISUALIZER_H

