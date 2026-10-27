// External baseline: the standard library's selection function.
// In libstdc++ (GCC) this is introselect: quickselect with a median-of-3
// pivot that falls back to a guaranteed method if recursion gets too deep.
// Check which standard library you are using and note it in the report.
#pragma once
#include <algorithm>

namespace selection {

template <typename It, typename Compare>
void baseline_select(It first, It kth, It last, Compare comp) {
    std::nth_element(first, kth, last, comp);
}

} // namespace selection
