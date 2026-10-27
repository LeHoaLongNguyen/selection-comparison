// Randomised quickselect
// Primary author: Andy Nguyen
//
// Plan (fill in as decisions are made, this helps the reflection later):
//   - Partition scheme: start with Lomuto, upgrade to three-way (Dutch national flag)
//   - Pivot choice: uniformly random index, RNG seeded explicitly (algorithm seed,
//     kept separate from the input seed)
//   - Recursion vs loop: TODO
#pragma once
#include <cstdint>
#include <stdexcept>

namespace selection {

template <typename It, typename Compare>
void quickselect(It first, It kth, It last, Compare comp, std::uint64_t seed = 1) {
    (void)first; (void)kth; (void)last; (void)comp; (void)seed;
    // TODO(Andy): implement. Remove the throw once it works.
    throw std::logic_error("quickselect: not implemented yet");
}

} // namespace selection
