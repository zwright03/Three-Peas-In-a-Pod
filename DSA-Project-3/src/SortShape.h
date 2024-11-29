#pragma once
#include "SFML/Graphics.hpp"

class SortShape : public sf::RectangleShape {
	int value;

public:
	SortShape() = default;
	SortShape(sf::Vector2f size);
	int getValue() const { return value; }
	void setValue(int newVal);
};

