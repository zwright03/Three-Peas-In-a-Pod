#include "SortVisualizer.h"
#include <algorithm>
#include <random>
#include <string>

Visualizer::Visualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case) : i(0), j(0), comparisons(0), swaps(0), xOffset(50),
yOffset(50), done(false), window(window)
{  // CREATES AND SHUFFLES OBJECTS TO SORT
	auto windowSize = window->getSize();
	
	int graphHeight = windowSize.y - yOffset * 2;
	float heightMultiplier = (float)graphHeight / (float)shapes.size(); // to normalize the heights, so that the largest item's height will be equal to the graph height

	float rectWidth = (windowSize.x - (xOffset * 4)) / (shapes.size() * 2);

	for (int i = 0; i < shapes.size(); i++) {
		int rectHeight = (i + 1) * heightMultiplier; // normalized height

		shapes[i] = SortShape(sf::Vector2f(rectWidth, rectHeight)); // SortShapes are initialized the same as sf::RectangleShape, all they do is store an extra value for sorting
		shapes[i].setValue(i + 1);
		shapes[i].setOrigin(0, rectHeight); // make the origin of the rectangle it's bottom left corner.
		shapes[i].setFillColor(sf::Color::White);
	}

	if (rightSide)
		xOffset += windowSize.x / 2; // draw on right side of window instead of left

	if (worst_case)
		std::reverse(shapes.begin(), shapes.end());
	else{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(shapes.begin(), shapes.end(), gen);
	}

	// initializes the text objects with everything theyll need
	swapsText.setFont(*font);
	swapsText.setCharacterSize(17);
	swapsText.setFillColor(sf::Color::White);
	swapsText.setPosition(sf::Vector2f(xOffset, windowSize.y - 45));

	comparisonsText.setFont(*font);
	comparisonsText.setCharacterSize(17);
	comparisonsText.setFillColor(sf::Color::White);
	comparisonsText.setPosition(sf::Vector2f(xOffset, windowSize.y - 28));
}

void Visualizer::draw() {
	auto windowSize = window->getSize();

	for (int i = 0; i < shapes.size(); i++) {
		shapes[i].setPosition(sf::Vector2f(xOffset + shapes[i].getSize().x * i, windowSize.y - yOffset));
		window->draw(shapes[i]);
	}

	std::string swapsString = "Swaps: " + std::to_string(swaps);
	std::string comparisonsString = "Comparisons: " + std::to_string(comparisons);

	swapsText.setString(swapsString);
	comparisonsText.setString(comparisonsString);

	window->draw(swapsText);
	window->draw(comparisonsText);
}

bool Visualizer::isDone() {
	return done;
}
