#include "BubbleVisualizer.h"

void BubbleVisualizer::iterate()
{
	// NORMAL BUBBLE SORT:
	// 
	// for (int i = 0; i < nums.size() - 1; i++) { 
	//	 for (int j = 0; j < nums.size() - i - 1; j++) {
	//	 	 if (nums[j] > nums[j + 1]) {
	//		 	 std::swap(nums[j], nums[j + 1]);
	//		 }
	//	 }
	// }

	if (!(i < shapes.size() - 1)) {
		done = true;
		return;
	}

	if (!(j < shapes.size() - i - 1)) {
		i++;
		j = 0;
		return;
	}
	
	if (shapes[j].getValue() > shapes[j + 1].getValue()) {
		std::swap(shapes[j], shapes[j + 1]);
		swaps++;
	}
	comparisons++;
	j++;
}
