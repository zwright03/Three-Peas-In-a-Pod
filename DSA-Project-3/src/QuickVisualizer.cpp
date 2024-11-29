#include "QuickVisualizer.h"

QuickVisualizer::QuickVisualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case)
        : Visualizer(window, font, rightSide, worst_case), pivotIndex(-1) {
    stack.push({0, static_cast<int>(shapes.size() - 1)});
}

void QuickVisualizer::iterate() {
    if (stack.empty()) {
        done = true;
        return;
    }

    auto [low, high] = stack.top();

    if (low < high) {
        if (pivotIndex == -1) {
            pivotIndex = low;
            int pivotValue = shapes[pivotIndex].getValue();
            int i = low + 1, j = high;

            while (i <= j) {
                if (shapes[i].getValue() <= pivotValue) {
                    i++;
                } else if (shapes[j].getValue() > pivotValue) {
                    j--;
                } else {
                    std::swap(shapes[i], shapes[j]);
                    swaps++;
                }
                comparisons++;
            }

            std::swap(shapes[low], shapes[j]);
            swaps++;

            stack.pop();
            stack.push({low, j - 1});
            stack.push({j + 1, high});
            pivotIndex = -1;
        }
    } else {
        stack.pop();
    }
}
