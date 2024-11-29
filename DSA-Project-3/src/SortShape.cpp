#include "SortShape.h"

SortShape::SortShape(sf::Vector2f size) : sf::RectangleShape(size), value(-1){}

void SortShape::setValue(int newVal) {
	this->value = newVal;
}
