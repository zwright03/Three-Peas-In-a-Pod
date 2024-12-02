#pragma once
#include "SortVisualizer.h"
#include <vector>

class MergeVisualizer : public Visualizer {
private:
    int current_size;
    int frame_count, update_frequency;
    void merge(std::array<SortShape, 50>& shapes, int left, int mid, int right);

public:
    MergeVisualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case = false);
    void iterate() override;
};


