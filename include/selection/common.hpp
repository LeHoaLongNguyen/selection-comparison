// Shared contract for every selection algorithm in this repo.
//
// Every algorithm has the same signature as std::nth_element:
//
//     template <typename It, typename Compare>
//     void algo(It first, It kth, It last, Compare comp);
//
// Postcondition (what the correctness test checks):
//   - *kth is the element that would be at kth if [first, last) were sorted
//   - every element in [first, kth) is <= *kth
//   - every element in (kth, last) is >= *kth
//
// k is 0-indexed. Duplicates are allowed.
#pragma once
#include <cstdint>

namespace selection {

// Wraps a comparator and counts how many times it is called.
// Use it ONLY for counting runs, never for timed runs: the counter
// increment sits in the inner loop and would distort the timings.
template <typename Compare>
struct CountingCompare {
    Compare comp;
    std::uint64_t* count;

    template <typename A, typename B>
    bool operator()(const A& a, const B& b) const {
        ++*count;
        return comp(a, b);
    }
};

} // namespace selection
