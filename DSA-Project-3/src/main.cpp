#include "DataPoint.h"
#include "MergeVisualizer.h"
#include "QuickVisualizer.h"
#include "SFML/Graphics.hpp"
#include "SortShape.h"
#include "SortVisualizer.h"
#include "timer.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <memory>
#include <mutex>
#include <thread>
#include <sstream>

// Mutex for multithreading
std::mutex mtx;

// Variables used for comparing performance
static thread_local size_t current_memory_usage = 0;
static thread_local size_t peak_memory_usage = 0;
static double time_merge_original, time_quick_original, time_merge_sorted, time_quick_sorted, time_merge_reverse, time_quick_reverse;
static size_t memory_merge_original, memory_quick_original, memory_merge_sorted, memory_quick_sorted, memory_merge_reverse, memory_quick_reverse;

// Methods used to test the max memory used by the sorting algorithms
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

// Data is not passed by reference, it needs to be copied for intended behaivor
static void runSortTest(std::vector<DataPoint> data, const std::string& test_name, void (*sort_fn)(std::vector<DataPoint>&, int, int), double& time_taken, size_t& memory_usage) {
    current_memory_usage = 0;
    peak_memory_usage = 0;

    time_taken = timeSortFunction(sort_fn, data);

    memory_usage = peak_memory_usage;

    std::lock_guard<std::mutex> lock(mtx);
    std::cout << test_name << " Performance: Time = " << time_taken / 1e6 << " seconds, Memory = " << memory_usage << " bytes\n";
}

// Tests all the different scenarios, calculating time and memory usage
static void testSortPerformance(std::vector<DataPoint>& original_data) {
    std::vector<DataPoint> sorted_data = original_data;
    std::sort(sorted_data.begin(), sorted_data.end(), [](const DataPoint& a, const DataPoint& b) {
        return a.total_amount < b.total_amount;});

    std::vector<DataPoint> reverse_sorted_data = sorted_data;
    std::reverse(reverse_sorted_data.begin(), reverse_sorted_data.end());

    std::array<std::thread, 6> threads;
    threads[0] = std::thread(runSortTest, original_data, "Merge Sort (Normal)", mergeSort, std::ref(time_merge_original), std::ref(memory_merge_original));
    threads[1] = std::thread(runSortTest, original_data, "Quick Sort (Normal)", quickSort, std::ref(time_quick_original), std::ref(memory_quick_original));
    threads[2] = std::thread(runSortTest, sorted_data, "Merge Sort (Already Sorted)", mergeSort, std::ref(time_merge_sorted), std::ref(memory_merge_sorted));
    threads[3] = std::thread(runSortTest, sorted_data, "Quick Sort (Already Sorted)", quickSort, std::ref(time_quick_sorted), std::ref(memory_quick_sorted));
    threads[4] = std::thread(runSortTest, reverse_sorted_data, "Merge Sort (Reverse Sorted)", mergeSort, std::ref(time_merge_reverse), std::ref(memory_merge_reverse));
    threads[5] = std::thread(runSortTest, reverse_sorted_data, "Quick Sort (Reverse Sorted)", quickSort, std::ref(time_quick_reverse), std::ref(memory_quick_reverse));

    for (auto& thread : threads) // Wait for all threads to finish before continuing
        thread.join();

}

// Rounds a provided float to n decimal places, returns a string
static std::string roundFloat(const double& num, const int& n) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(n) << num;
    return ss.str();
}

// Main function that calls the performance method and all of the SFML windows
int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Sorting Visualizer");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("resources/fonts/Roboto-Bold.ttf")) {
        std::cerr << "ERROR: Missing font file" << std::endl;
        return -1;
    }

    { // Welcome and introduction window
        window.setTitle("Merge Sort vs. Quick Sort Introduction");
        std::string welcomeStr = 
R"(Welcome to the sorting visualizer.
This will be a comparison between the performance of
the Merge sort and Quick sort algorithms.

We will start with a visualization of Merge sort and Quick sort
simultaneously, showing where swaps are made in red. Then, we
will calculate the time and memory usage of each sorting algorithm
on a large database of 125,000 entries. Finally, we will display
these results using bar graphs for three different scenarios:
One where the data is randomly unsorted, one where it is already
sorted, and one where it is sorted in reverse.

One problem we are solving with our project is some systems using
a specific sorting algorithm without understanding its strengths
and shortcomings. This causes inefficiencies, especially when the
databases are extremely large, containing over 100,000 entries.
With our analysis we will determine the optimal algorithm based on
the system's foundation and needs.

Click anywhere to continue.)";

        sf::Text welcomeText;
        welcomeText.setFont(font);
        welcomeText.setString(welcomeStr);
        welcomeText.setCharacterSize(25);
        welcomeText.setFillColor(sf::Color::White);
        welcomeText.setOrigin(welcomeText.getLocalBounds().width / 2, welcomeText.getLocalBounds().height / 2);
        welcomeText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

        bool move_to_visualization = false;
        while (window.isOpen() && !move_to_visualization) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::MouseButtonPressed)
                    move_to_visualization = true;
            }

            window.clear(sf::Color::Black);
            window.draw(welcomeText);
            window.display();
        }
    }


    if (!window.isOpen()) // if the window was closed during the above part, then exit
        return 0;


    { // Sorting visualization window
        window.setTitle("Merge Sort vs. Quick Sort Visualization");

        sf::Text titleText;
        titleText.setFont(font);
        titleText.setString("Merge Sort vs. Quick Sort Visualization:");
        titleText.setCharacterSize(35);
        titleText.setFillColor(sf::Color::White);
        titleText.setOrigin(titleText.getLocalBounds().width / 2, titleText.getLocalBounds().height / 2);
        titleText.setPosition(window.getSize().x / 2, titleText.getLocalBounds().height / 2);

        sf::Text nextText;
        nextText.setFont(font);
        nextText.setString("Click anywhere to continue.");
        nextText.setCharacterSize(25);
        nextText.setFillColor(sf::Color::Blue);
        nextText.setOrigin(nextText.getLocalBounds().width / 2, nextText.getLocalBounds().height / 2);
        nextText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

        auto vis1 = std::make_shared<QuickVisualizer>(&window, &font, true); // Right side
        auto vis2 = std::make_shared<MergeVisualizer>(&window, &font, false); // Left side

        bool move_to_results = false;
        while (window.isOpen() && !move_to_results) {
            bool vis1Done = vis1->isDone();
            bool vis2Done = vis2->isDone();

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::MouseButtonPressed && vis1Done && vis2Done) 
                    move_to_results = true;
            }

            if (!vis1Done)
                vis1->iterate();

            if (!vis2Done)
                vis2->iterate();

            window.clear(sf::Color::Black);

            window.draw(titleText);
            vis1->draw();
            vis2->draw();
            if (vis1Done && vis2Done)
                window.draw(nextText);

            window.display();
        }
    }


    if (!window.isOpen()) // if the window was closed during the above part, then exit
        return 0;


    { // screen to display while loading and sorting the data.
        sf::Text loadingText;
        loadingText.setFont(font);
        loadingText.setString("Currently comparing the performance of merge sort and quick sort.\nPlease wait.");
        loadingText.setCharacterSize(35);
        loadingText.setFillColor(sf::Color::White);
        loadingText.setOrigin(loadingText.getLocalBounds().width / 2, loadingText.getLocalBounds().height / 2);
        loadingText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

        window.clear(sf::Color::Black);
        window.draw(loadingText);
        window.display();
    }

    // Calculate the values of the performance on the large database
    if (window.isOpen()) {
        try {
            std::vector<DataPoint> data = loadData();
            testSortPerformance(data);
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    else
        return 0;

    { // Large database comparison results window
        sf::Text text1("Original unsorted", font, 30);
        sf::Text text2("Already sorted", font, 30);
        sf::Text text3("Reverse sorted", font, 30);

        float windowWidth = window.getSize().x;
        float windowHeight = window.getSize().y;
        float yPosition = windowHeight - 100;

        float spacing = windowWidth / 4;
        text1.setPosition(spacing - text1.getLocalBounds().width / 2 - 90, yPosition - 615);
        text2.setPosition(2 * spacing - text2.getLocalBounds().width / 2 - 15, yPosition - 615);
        text3.setPosition(3 * spacing - text3.getLocalBounds().width / 2 + 60, yPosition - 615);

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

        sf::Text time1(roundFloat(time_merge_original / 1e6, 2) + 'S' + "\nTime", font, 20);
        sf::Text memory1(roundFloat((double)memory_merge_original / 1e6, 1) + "MB" + "\nMemory", font, 20);

        sf::Text time2(roundFloat(time_quick_original / 1e6, 2) + 'S' + "\nTime", font, 20);
        sf::Text memory2(roundFloat((double)memory_quick_original / 1e6, 1) + "MB" + "\nMemory", font, 20);

        sf::Text time3(roundFloat(time_merge_sorted / 1e6, 2) + 'S' + "\nTime", font, 20);
        sf::Text memory3(roundFloat((double)memory_merge_sorted / 1e6, 1) + "MB" + "\nMemory", font, 20);

        sf::Text time4(roundFloat(time_quick_sorted / 1e6, 2) + 'S' + "\nTime", font, 20);
        sf::Text memory4(roundFloat((double)memory_quick_sorted / 1e6, 1) + "MB" + "\nMemory", font, 20);

        sf::Text time5(roundFloat(time_merge_reverse / 1e6, 2) + 'S' + "\nTime", font, 20);
        sf::Text memory5(roundFloat((double)memory_merge_reverse / 1e6, 1) + "MB" + "\nMemory", font, 20);

        sf::Text time6(roundFloat(time_quick_reverse / 1e6, 2) + 'S' + "\nTime", font, 20);
        sf::Text memory6(roundFloat((double)memory_quick_reverse / 1e6, 1) + "MB" + "\nMemory", font, 20);

        float timeMemoryOffset = 60;
        float columnOffset = 50;

        time1.setPosition(spacing - label1.getLocalBounds().width / 2 - 75 - columnOffset - 15, yPosition - labelOffset - timeMemoryOffset);
        memory1.setPosition(spacing - label1.getLocalBounds().width / 2 - 75 - columnOffset + 60, yPosition - labelOffset - timeMemoryOffset);
        time2.setPosition(spacing - label1.getLocalBounds().width / 2 - 75 - columnOffset + 165, yPosition - labelOffset - timeMemoryOffset);
        memory2.setPosition(spacing - label1.getLocalBounds().width / 2 - 75 - columnOffset + 240, yPosition - labelOffset - timeMemoryOffset);
        time3.setPosition(2 * spacing - label1.getLocalBounds().width / 2 - columnOffset - 15, yPosition - labelOffset - timeMemoryOffset);
        memory3.setPosition(2 * spacing - label1.getLocalBounds().width / 2 - columnOffset + 60, yPosition - labelOffset - timeMemoryOffset);
        time4.setPosition(2 * spacing - label1.getLocalBounds().width / 2 - columnOffset + 165, yPosition - labelOffset - timeMemoryOffset);
        memory4.setPosition(2 * spacing - label1.getLocalBounds().width / 2 - columnOffset + 240, yPosition - labelOffset - timeMemoryOffset);
        time5.setPosition(3 * spacing - label1.getLocalBounds().width / 2 + 75 - columnOffset - 15, yPosition - labelOffset - timeMemoryOffset);
        memory5.setPosition(3 * spacing - label1.getLocalBounds().width / 2 + 75 - columnOffset + 60, yPosition - labelOffset - timeMemoryOffset);
        time6.setPosition(3 * spacing - label1.getLocalBounds().width / 2 + 75 - columnOffset + 165, yPosition - labelOffset - timeMemoryOffset);
        memory6.setPosition(3 * spacing - label1.getLocalBounds().width / 2 + 75 - columnOffset + 240, yPosition - labelOffset - timeMemoryOffset);

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

        std::array<double, 6> timeValues = { time_merge_original, time_merge_reverse, time_merge_sorted, time_quick_original, time_quick_reverse, time_quick_sorted };
        std::array<size_t, 6> memoryValues = { memory_merge_original, memory_merge_reverse, memory_merge_sorted, memory_quick_original, memory_quick_reverse, memory_quick_sorted };
        double maxTime = *std::max_element(timeValues.begin(), timeValues.end());
        double maxMemory = *std::max_element(memoryValues.begin(), memoryValues.end());

        double timeScaler = 350 / maxTime;
        double memoryScaler = 350 / maxMemory;

        std::vector<double> barHeights = {
                time_merge_original * timeScaler,
                memory_merge_original * memoryScaler,
                time_quick_original * timeScaler,
                memory_quick_original * memoryScaler,
                time_merge_sorted * timeScaler,
                memory_merge_sorted * memoryScaler,
                time_quick_sorted * timeScaler,
                memory_quick_sorted * memoryScaler,
                time_merge_reverse * timeScaler,
                memory_merge_reverse * memoryScaler,
                time_quick_reverse * timeScaler,
                memory_quick_reverse * memoryScaler
        };

        std::vector<sf::Color> barColors = {
                sf::Color::Blue, sf::Color::Green,
                sf::Color::Blue, sf::Color::Green,
                sf::Color::Blue, sf::Color::Green,
                sf::Color::Blue, sf::Color::Green,
                sf::Color::Blue, sf::Color::Green,
                sf::Color::Blue, sf::Color::Green
        };
        std::vector<float> barXPositions = {
                spacing - label1.getLocalBounds().width / 2 - 66 - columnOffset - 7,
                spacing - label1.getLocalBounds().width / 2 - 66 - columnOffset + 62,
                spacing - label1.getLocalBounds().width / 2 - 66 - columnOffset + 173,
                spacing - label1.getLocalBounds().width / 2 - 66 - columnOffset + 242,
                2 * spacing - label1.getLocalBounds().width / 2 + 9 - columnOffset - 7,
                2 * spacing - label1.getLocalBounds().width / 2 + 9 - columnOffset + 62,
                2 * spacing - label1.getLocalBounds().width / 2 + 9 - columnOffset + 173,
                2 * spacing - label1.getLocalBounds().width / 2 + 9 - columnOffset + 242,
                3 * spacing - label1.getLocalBounds().width / 2 + 84 - columnOffset - 7,
                3 * spacing - label1.getLocalBounds().width / 2 + 84 - columnOffset + 62,
                3 * spacing - label1.getLocalBounds().width / 2 + 84 - columnOffset + 173,
                3 * spacing - label1.getLocalBounds().width / 2 + 84 - columnOffset + 242
        };
        std::vector<sf::RectangleShape> bars;
        for (size_t i = 0; i < barHeights.size(); ++i) {
            sf::RectangleShape bar(sf::Vector2f(30, -barHeights[i]));
            bar.setPosition(barXPositions[i], yPosition - labelOffset - timeMemoryOffset - 20);
            bar.setFillColor(barColors[i % barColors.size()]);
            bars.push_back(bar);
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::Black);

            window.draw(text1);
            window.draw(text2);
            window.draw(text3);
            window.draw(label1);
            window.draw(label2);
            window.draw(label3);
            window.draw(time1);
            window.draw(memory1);
            window.draw(time2);
            window.draw(memory2);
            window.draw(time3);
            window.draw(memory3);
            window.draw(time4);
            window.draw(memory4);
            window.draw(time5);
            window.draw(memory5);
            window.draw(time6);
            window.draw(memory6);

            for (auto& bar : bars) {
                window.draw(bar);
            }

            window.display();
        }
    }
    return 0;
}
