// Randomised quickselect
// Primary author: Andy Nguyen
#pragma once
#include <cstddef>   
#include <cstdint>   
#include <random>    
#include <utility>   

namespace selection {

template <typename It, typename Compare>
void quickselect(It first, It kth, It last, Compare comp, std::uint64_t seed = 1) {
	const std::ptrdiff_t n = last - first;
	if (n <= 1) return;
	const std::ptrdiff_t k = kth - first;
	std::ptrdiff_t left = 0, right = n - 1;
	std::mt19937_64 rng(seed);

	while (left < right) {
		//1. pick a random pivot in [left, right] and copy the value
		std::ptrdiff_t pivot_index = left + rng() % (right - left + 1);
		auto pivot = first[pivot_index];

		//2. three-way partition of [left, right]:
		// [left, lt] < pivot || [lt, i] == pivot | [gt, right] > pivot
		std::ptrdiff_t lt = left, i = left, gt = right;
		while (i <= gt) {
			if (comp(first[i], pivot)) {
				std::swap(first[i], first[lt]);
				++lt;
				++i;
			}
			else if (comp(pivot, first[i])) {
				std::swap(first[i], first[gt]);
				--gt;
			}
			else {
				++i;
			}
		}
		// 3. keep only the part that contains k
		if (k < lt) {
			right = lt - 1;
		}
		else if (k > gt) {
			left = gt + 1;
		}
		else {
			return;
		}
	}
	
    
}

} //namespace selection
