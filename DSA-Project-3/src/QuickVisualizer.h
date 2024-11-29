#ifndef DSA_PROJECT_3_QUICKVISUALIZER_H
#define DSA_PROJECT_3_QUICKVISUALIZER_H
#pragma once
#include "SortVisualizer.h"
#include <stack>

class QuickVisualizer : public Visualizer {
private:
    std::stack<std::pair<int, int>> stack;
    int pivotIndex;

public:
    QuickVisualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case = false);
    void iterate() override;
};

#endif //DSA_PROJECT_3_QUICKVISUALIZER_H
