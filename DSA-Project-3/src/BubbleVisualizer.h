#pragma once
#include "SortVisualizer.h"


class BubbleVisualizer : public Visualizer
{
public:
	BubbleVisualizer(sf::RenderWindow* window, sf::Font* font, bool leftSide, bool worst_case=false) : Visualizer(window, font, leftSide, worst_case){}
	void iterate();
};

