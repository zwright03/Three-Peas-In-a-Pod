#pragma once
#include "SFML/Graphics.hpp"
#include "SortShape.h"
#include <array>

class Visualizer{
private:
	int xOffset, yOffset; // In pixels
	sf::RenderWindow* window; // Pointer to the window to draw on
	sf::Text swapsText;
	sf::Text comparisonsText;

protected:
	std::array<SortShape, 50> shapes; // Array of shapes to draw and sort, using n = 50
	int i, j, comparisons, swaps; // i and j for looping, comparisons and swaps for comparing the algorithms
	bool done; // To check if it's done sorting

public:
	Visualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case=false);
	void draw();
	bool isDone();
	virtual void iterate() = 0;
};
