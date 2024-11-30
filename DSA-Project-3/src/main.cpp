#include "DataPoint.h"
#include "MergeVisualizer.h"
#include "QuickVisualizer.h"
#include "SFML/Graphics.hpp"
#include "SortShape.h"
#include "SortVisualizer.h"
#include "timer.h"
#include <iostream>
#include <memory>

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

    return 0;
}