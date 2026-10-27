// Deterministic median-of-medians selection (BFPRT)
// Primary author: Joshua
//
// Plan (fill in as decisions are made, this helps the reflection later):
//   - Group size: 5
//   - Partition scheme: agree with Andy so both algorithms share the same partition,
//     so the only difference measured is how the pivot is chosen
//   - Small-n cutoff (e.g. insertion sort below some size): TODO
#pragma once
#include <stdexcept>

namespace selection {

template <typename It, typename Compare>
void median_of_medians(It first, It kth, It last, Compare comp) {
    (void)first; (void)kth; (void)last; (void)comp;
    // TODO(Joshua): implement. Remove the throw once it works.
    throw std::logic_error("median_of_medians: not implemented yet");
}

} // namespace selection
