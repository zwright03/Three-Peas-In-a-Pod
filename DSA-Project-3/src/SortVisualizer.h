#pragma once
#include "SFML/Graphics.hpp"
#include "SortShape.h"
#include <array>


class Visualizer{
private:
	int xOffset, yOffset; // in pixels
	sf::RenderWindow* window; // pointer to the window to draw on

	sf::Text swapsText;
	sf::Text comparisonsText;

protected:
	std::array<SortShape, 50> shapes; // array of shapes to draw and sort, using n = 50
	int i, j, comparisons, swaps; // i and j for looping, comparisons and swaps for comparing against other algorithms
	bool done; // to check if its done sorting outside of the class. If it is, dont call iterate.

public:
	Visualizer(sf::RenderWindow* window, sf::Font* font, bool rightSide, bool worst_case=false);
	void draw();
	bool isDone();
	virtual void iterate() = 0;
};
