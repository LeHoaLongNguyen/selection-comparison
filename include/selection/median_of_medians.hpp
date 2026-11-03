// Median-of-medians selection (deterministic, worst case O(n))
// Primary author: Joshua Davis
#pragma once
#include <algorithm>

namespace selection {

template <class It, class Compare>
void mom_insertion_sort(It first, It last, Compare comp) {
    for (It i = first; i != last; ++i)
        for (It j = i; j != first && comp(*j, *(j - 1)); --j)
            std::iter_swap(j, j - 1);
}

template <class It, class Compare>
void median_of_medians(It first, It kth, It last, Compare comp) {
    while (last - first > 5) {
        // 1. sort each group of 5 and move its median to the front
        auto n = last - first;
        It store = first;
        for (decltype(n) i = 0; i < n; i += 5) {
            It g = first + i;
            It g_end = first + std::min(i + 5, n);
            mom_insertion_sort(g, g_end, comp);
            std::iter_swap(store++, g + (g_end - g - 1) / 2);
        }

        // 2. pivot = median of the medians (recursive)
        It mid = first + (store - first - 1) / 2;
        median_of_medians(first, mid, store, comp);
        auto pivot = *mid;

        // 3. three-way partition: [< pivot][== pivot][> pivot]
        It lt = first, i = first, gt = last;
        while (i < gt) {
            if (comp(*i, pivot)) std::iter_swap(lt++, i++);
            else if (comp(pivot, *i)) std::iter_swap(i, --gt);
            else ++i;
        }

        // 4. keep only the part that contains kth
        if (kth < lt) last = lt;
        else if (kth >= gt) first = gt;
        else return;
    }
    mom_insertion_sort(first, last, comp);
}

}  // namespace selection