#include "DataPoint.h"
#include "timer.h"
#include <iostream>

#include "BubbleVisualizer.h"
#include "SelectionVisualizer.h"
#include "SFML/Graphics.hpp"
#include "SortVisualizer.h"
#include <memory>

int main() {
	{
		Timer timer;
		std::vector<DataPoint> data_points = loadData();
	}

	// Algorithm Visualizer Main Code:
	// 
//	sf::RenderWindow window(sf::VideoMode(1200, 800), "Algorithm Visualizer");
//	window.setFramerateLimit(60);
//
//	sf::Font font;
//	if (!font.loadFromFile("resources/fonts/Roboto-Bold.ttf")) {
//		std::cout << "ERROR MISSING FONT FILE" << std::endl;
//		return -1;
//	}
//
//	//Text creation
//	//sf::Text testText;
//	//testText.setFont(font);
//	//testText.setString("Test");
//	//testText.setCharacterSize(50);
//	//testText.setFillColor(sf::Color::Black);
//	//testText.setPosition(500, 350);
//
//	std::shared_ptr<BubbleVisualizer> vis1 = std::make_shared<BubbleVisualizer>(&window, &font, true);
//	std::shared_ptr<SelectionVisualizer> vis2 = std::make_shared<SelectionVisualizer>(&window, &font, false);
//
//	while (window.isOpen())
//	{
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//				window.close();
//		}
//
//		if (!vis1->isDone())
//			vis1->iterate();
//
//		if (!vis2->isDone())
//			vis2->iterate();
//
//		window.clear(sf::Color::Black);
//		vis1->draw();
//		vis2->draw();
//		window.display();
//	}

	return 0;
}