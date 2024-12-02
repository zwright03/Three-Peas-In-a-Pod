#pragma once
#include "SortVisualizer.h"
#include <stack>

class QuickVisualizer : public Visualizer {
private:
    std::stack<std::pair<int, int>> stack;
    int pivotIndex;
    int frame_count, update_frequency;

public:
    QuickVisualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case = false);
    void iterate() override;
};

