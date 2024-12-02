#include "DataPoint.h"
#include "MergeVisualizer.h"
#include "QuickVisualizer.h"
#include "SFML/Graphics.hpp"
#include "SortShape.h"
#include "SortVisualizer.h"
#include "timer.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <cstddef>
#include <cstdlib>

size_t current_memory_usage = 0;
size_t peak_memory_usage = 0;
double time_merge_original, time_quick_original, time_merge_sorted, time_quick_sorted, time_merge_reverse, time_quick_reverse;
size_t memory_merge_original, memory_quick_original, memory_merge_sorted, memory_quick_sorted, memory_merge_reverse, memory_quick_reverse;

void* operator new(size_t size) {
    current_memory_usage += size;
    if (current_memory_usage > peak_memory_usage)
        peak_memory_usage = current_memory_usage;
    return malloc(size);
}

void operator delete(void* ptr, size_t size) noexcept {
    current_memory_usage -= size;
    free(ptr);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void testSortPerformance(std::vector<DataPoint>& original_data) {
    std::vector<DataPoint> data;

    std::cout << "\nInitial Memory Usage: " << current_memory_usage << " bytes\n";

    data = original_data;
    current_memory_usage = 0;
    peak_memory_usage = 0;
    Timer timer;
    timer.start();
    mergeSort(data, 0, data.size() - 1);
    int time_merge_original = timer.stop();
    std::cout << "\nOriginal sorted dataset testing:" << std::endl;
    std::cout << "Merge Sort: Time = " << time_merge_original / 1e6 << " seconds, " << "Memory = " << peak_memory_usage << " bytes\n";
    size_t memory_merge_original = peak_memory_usage;

    data = original_data;
    current_memory_usage = 0;
    peak_memory_usage = 0;
    timer.start();
    quickSort(data, 0, data.size() - 1);
    int time_quick_original = timer.stop();
    std::cout << "Quick Sort: Time = " << time_quick_original / 1e6 << " seconds, " << "Memory = " << peak_memory_usage << " bytes\n";
    size_t memory_quick_original = peak_memory_usage;

    data = original_data;
    std::sort(data.begin(), data.end(), [](const DataPoint& a, const DataPoint& b) {
        return a.total_amount < b.total_amount;
    });
    current_memory_usage = 0;
    peak_memory_usage = 0;
    timer.start();
    mergeSort(data, 0, data.size() - 1);
    int time_merge_sorted = timer.stop();
    std::cout << "\nAlready sorted dataset testing:" << std::endl;
    std::cout << "Merge Sort: Time = " << time_merge_sorted / 1e6 << " seconds, " << "Memory = " << peak_memory_usage << " bytes\n";
    size_t memory_merge_sorted = peak_memory_usage;

    data = original_data;
    std::sort(data.begin(), data.end(), [](const DataPoint& a, const DataPoint& b) {
        return a.total_amount < b.total_amount;
    });
    current_memory_usage = 0;
    peak_memory_usage = 0;
    timer.start();
    quickSort(data, 0, data.size() - 1);
    int time_quick_sorted = timer.stop();
    std::cout << "Quick Sort: Time = " << time_quick_sorted / 1e6 << " seconds, " << "Memory = " << peak_memory_usage << " bytes\n";
    size_t memory_quick_sorted = peak_memory_usage;

    data = original_data;
    std::sort(data.begin(), data.end(), [](const DataPoint& a, const DataPoint& b) {
        return a.total_amount > b.total_amount;
    });
    current_memory_usage = 0;
    peak_memory_usage = 0;
    timer.start();
    mergeSort(data, 0, data.size() - 1);
    int time_merge_reverse = timer.stop();
    std::cout << "\nReverse sorted dataset testing:" << std::endl;
    std::cout << "Merge Sort: Time = " << time_merge_reverse / 1e6 << " seconds, " << "Memory = " << peak_memory_usage << " bytes\n";
    size_t memory_merge_reverse = peak_memory_usage;

    data = original_data;
    std::sort(data.begin(), data.end(), [](const DataPoint& a, const DataPoint& b) {
        return a.total_amount > b.total_amount;
    });
    current_memory_usage = 0;
    peak_memory_usage = 0;
    timer.start();
    quickSort(data, 0, data.size() - 1);
    int time_quick_reverse = timer.stop();
    std::cout << "Quick Sort: Time = " << time_quick_reverse / 1e6 << " seconds, " << "Memory = " << peak_memory_usage << " bytes\n";
    size_t memory_quick_reverse = peak_memory_usage;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Sorting Visualizer");
    window.setFramerateLimit(60); // there are fields in the classes for the visualuzers to slow them down
    // set update_frequency in each class higher to slow it down, or lower to speed it up

    sf::Font font;
    if (!font.loadFromFile("resources/fonts/Roboto-Bold.ttf")) {
        std::cerr << "ERROR: Missing font file" << std::endl;
        return -1;
    }

    sf::Text testText;
    testText.setFont(font);
    testText.setString("Test");
    testText.setCharacterSize(50);
    testText.setFillColor(sf::Color::Black);
    testText.setPosition(500, 350);

    auto vis1 = std::make_shared<QuickVisualizer>(&window, &font, true); // right side
    auto vis2 = std::make_shared<MergeVisualizer>(&window, &font, false); // left side

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!vis1->isDone())
            vis1->iterate();

        if (!vis2->isDone())
            vis2->iterate();

        window.clear(sf::Color::Black);
        vis1->draw();
        vis2->draw();
        window.display();
    }

    try {
        std::vector<DataPoint> data = loadData();
        testSortPerformance(data);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // enter SFML here
    return 0;
}