#pragma once
#include "SortVisualizer.h"
class SelectionVisualizer : public Visualizer
{
private:
	int updateFrequency; // selection sort runs too fast, so it will be iterated once every updateFrequency frames
	int frameCount;

public:
	SelectionVisualizer(sf::RenderWindow* window, sf::Font* font, bool leftSide, bool worst_case = false);
	void iterate();
};

