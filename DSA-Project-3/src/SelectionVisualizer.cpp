#include "SelectionVisualizer.h"

SelectionVisualizer::SelectionVisualizer(sf::RenderWindow* window, sf::Font* font, bool leftSide, bool worst_case) : Visualizer(window, font, leftSide, worst_case),
																					updateFrequency(15), frameCount(0){}

void SelectionVisualizer::iterate()
{
	// NORMAL SELECTION SORT:
	//
	//for (int i = 0; i < nums.size() - 1; i++) {
	//	int min_idx = i;
	//	for (int j = i + 1; j < nums.size(); j++) {
	//		if (nums[j] > nums[min_idx])
	//			min_idx = j;
	//	}
	//	std::swap(nums[min_idx], nums[j]);
	//}

	frameCount++; // selection sort runs too fast, so it will be iterated once every updateFrequency frames
	if (frameCount >= 100000) // to eliminate the chance of overflow
		frameCount = 0;
	if (frameCount % updateFrequency != 0)
		return;

	if (!(i < shapes.size() - 1)) {
		done = true;
		return;
	}

	int min_idx = i;
	for (j = i + 1; j < shapes.size(); j++) {
		if (shapes[j].getValue() < shapes[min_idx].getValue())
				min_idx = j;		
		comparisons++;
	}
	
	if(i != min_idx){
		std::swap(shapes[min_idx], shapes[i]);
		swaps++;
	}

	i++;
}
