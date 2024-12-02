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

    /*
    try {
        std::vector<DataPoint> data = loadData();
        testSortPerformance(data);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
     */

    // Comparison window
    sf::RenderWindow resultWindow(sf::VideoMode(1200, 800), "Sorting Comparisons");

    sf::Text text1("Original unsorted", font, 30);
    sf::Text text2("Already sorted", font, 30);
    sf::Text text3("Reverse sorted", font, 30);

    float windowWidth = resultWindow.getSize().x;
    float windowHeight = resultWindow.getSize().y;
    float yPosition = windowHeight - 100;

    float spacing = windowWidth / 4;
    text1.setPosition(spacing - text1.getLocalBounds().width / 2 - 75, yPosition);
    text2.setPosition(2 * spacing - text2.getLocalBounds().width / 2, yPosition);
    text3.setPosition(3 * spacing - text3.getLocalBounds().width / 2 + 75, yPosition);

    text1.setFillColor(sf::Color::White);
    text2.setFillColor(sf::Color::White);
    text3.setFillColor(sf::Color::White);

    sf::Text label1("Merge   vs.   Quick", font, 25);
    sf::Text label2("Merge   vs.   Quick", font, 25);
    sf::Text label3("Merge   vs.   Quick", font, 25);

    float labelOffset = 60;

    label1.setPosition(spacing - label1.getLocalBounds().width / 2 - 75, yPosition - labelOffset);
    label2.setPosition(2 * spacing - label2.getLocalBounds().width / 2, yPosition - labelOffset);
    label3.setPosition(3 * spacing - label3.getLocalBounds().width / 2 + 75, yPosition - labelOffset);

    label1.setFillColor(sf::Color::White);
    label2.setFillColor(sf::Color::White);
    label3.setFillColor(sf::Color::White);

    sf::Text time1("Time", font, 20);
    sf::Text memory1("Memory", font, 20);
    sf::Text time2("Time", font, 20);
    sf::Text memory2("Memory", font, 20);
    sf::Text time3("Time", font, 20);
    sf::Text memory3("Memory", font, 20);
    sf::Text time4("Time", font, 20);
    sf::Text memory4("Memory", font, 20);
    sf::Text time5("Time", font, 20);
    sf::Text memory5("Memory", font, 20);
    sf::Text time6("Time", font, 20);
    sf::Text memory6("Memory", font, 20);

    float timeMemoryOffset = 60;
    float columnOffset = 50;

    time1.setPosition(spacing - label1.getLocalBounds().width / 2 - 75 - columnOffset - 7, yPosition - labelOffset - timeMemoryOffset);
    memory1.setPosition(spacing - label1.getLocalBounds().width / 2 - 75 - columnOffset + 53, yPosition - labelOffset - timeMemoryOffset);
    time2.setPosition(spacing - label1.getLocalBounds().width / 2 - 75 - columnOffset + 173, yPosition - labelOffset - timeMemoryOffset);
    memory2.setPosition(spacing - label1.getLocalBounds().width / 2 - 75 - columnOffset + 233, yPosition - labelOffset - timeMemoryOffset);
    time3.setPosition(2*spacing - label1.getLocalBounds().width / 2 - columnOffset - 7, yPosition - labelOffset - timeMemoryOffset);
    memory3.setPosition(2*spacing - label1.getLocalBounds().width / 2 - columnOffset + 53, yPosition - labelOffset - timeMemoryOffset);
    time4.setPosition(2*spacing - label1.getLocalBounds().width / 2 - columnOffset + 173, yPosition - labelOffset - timeMemoryOffset);
    memory4.setPosition(2*spacing - label1.getLocalBounds().width / 2 - columnOffset + 233, yPosition - labelOffset - timeMemoryOffset);
    time5.setPosition(3*spacing - label1.getLocalBounds().width / 2 + 75 - columnOffset - 7, yPosition - labelOffset - timeMemoryOffset);
    memory5.setPosition(3*spacing - label1.getLocalBounds().width / 2 + 75 - columnOffset + 53, yPosition - labelOffset - timeMemoryOffset);
    time6.setPosition(3*spacing - label1.getLocalBounds().width / 2 + 75 - columnOffset + 173, yPosition - labelOffset - timeMemoryOffset);
    memory6.setPosition(3*spacing - label1.getLocalBounds().width / 2 + 75 - columnOffset + 233, yPosition - labelOffset - timeMemoryOffset);

    time1.setFillColor(sf::Color::White);
    memory1.setFillColor(sf::Color::White);
    time2.setFillColor(sf::Color::White);
    memory2.setFillColor(sf::Color::White);
    time3.setFillColor(sf::Color::White);
    memory3.setFillColor(sf::Color::White);
    time4.setFillColor(sf::Color::White);
    memory4.setFillColor(sf::Color::White);
    time5.setFillColor(sf::Color::White);
    memory5.setFillColor(sf::Color::White);
    time6.setFillColor(sf::Color::White);
    memory6.setFillColor(sf::Color::White);

    while (resultWindow.isOpen()) {
        sf::Event event;
        while (resultWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                resultWindow.close();
        }

        resultWindow.clear(sf::Color::Black);

        resultWindow.draw(text1);
        resultWindow.draw(text2);
        resultWindow.draw(text3);
        resultWindow.draw(label1);
        resultWindow.draw(label2);
        resultWindow.draw(label3);
        resultWindow.draw(time1);
        resultWindow.draw(memory1);
        resultWindow.draw(time2);
        resultWindow.draw(memory2);
        resultWindow.draw(time3);
        resultWindow.draw(memory3);
        resultWindow.draw(time4);
        resultWindow.draw(memory4);
        resultWindow.draw(time5);
        resultWindow.draw(memory5);
        resultWindow.draw(time6);
        resultWindow.draw(memory6);
        // ADD BAR GRAPHS

        resultWindow.display();
    }

    return 0;
}