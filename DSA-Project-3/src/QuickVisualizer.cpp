#include "QuickVisualizer.h"

// This will visualize the process of the quick sort (right side of screen)
QuickVisualizer::QuickVisualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case)
        : Visualizer(window, font, rightSide, worst_case), pivotIndex(-1), update_frequency(10), frame_count(0) {
    stack.push({0, static_cast<int>(shapes.size() - 1)});
}

void QuickVisualizer::iterate() {
    frame_count++; // To slow it down when running so process is easier to see
    if (frame_count % update_frequency != 0)
        return;
    if (frame_count > 1000000)
        frame_count = 0;
    if (stack.empty()) {
        done = true;
        return;
    }

    for(int i = 0; i < 50; i++){
        shapes[i].setFillColor(sf::Color::White);
    }

    auto [low, high] = stack.top();
    if (low < high) {
        if (pivotIndex == -1) {
            pivotIndex = low;
            shapes[low].setFillColor(sf::Color::Red);
            shapes[high].setFillColor(sf::Color::Red);
            // Coloring red to help visualize the process
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
