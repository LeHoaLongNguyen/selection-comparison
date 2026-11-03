# Predictions (written before running any experiments)

# Predictions

Written before running any benchmarks. These are our honest expectations and will not be edited after results come in.

Note: the starter skeleton ran `std::nth_element` once as a build check, but we had not looked at any timing results when writing this.

## Theoretical comparison

**Time complexity.** Both algorithms run in O(n) on average. Median-of-medians is also O(n) in the worst case, because its pivot always has at least about 3n/10 elements on each side. Randomised quickselect is O(n) expected but O(n²) in the worst case if it keeps picking bad pivots.

**Constant factors.** Median-of-medians does much more work per round than quickselect. Before it can partition, it has to sort every group of five (touching every element), then recursively find the median of the n/5 group medians. Quickselect just picks a random pivot and partitions straight away.

**Memory.** Both algorithms work in place on the input array, so neither needs a second array. The only extra memory is the recursion stack. In our median-of-medians, the outer search is a loop and only the median-of-medians step recurses, each time on a fifth of the size, so the stack depth is O(log n) (about 9 levels for n = 1,000,000). Quickselect needs O(log n) stack on average if written recursively, but up to O(n) in the worst case with repeated bad pivots, or almost none if written as a loop.

**Input sensitivity.** Median-of-medians is deterministic, so for the same input it always does exactly the same steps. Quickselect's pivots come from a random number generator, not the input, so sorted or reversed order should not affect it. Both are sensitive to duplicate values depending on how they partition.

## Predictions

1. **Random data.** Quickselect will be faster than median-of-medians, by around 2×, because of the extra group sorting and recursion described above.

2. **`std::nth_element`.** It will be faster than both of our implementations. It is a heavily optimised standard library implementation, so it likely uses better pivot selection and partitioning, and should perform well on all input types.

3. **Input types.**
   - Median-of-medians will perform similarly across most inputs, within about 1.5× of its random-input time. All-equal data might be faster, because the three-way partition puts every element in the "equal" section in one pass.
   - Quickselect should be unaffected by sorted or reversed input, since its random pivots don't depend on input order. Few-distinct and all-equal inputs could cause problems depending on how its partition handles duplicate values.

4. **Run-to-run variation.**
   - Quickselect's slowest runs will be noticeably slower than its typical runs, around 2× or more, depending on the pivots it happens to pick.
   - Median-of-medians will be much more consistent. Since it does identical work on the same input every time, any variation should only be timing noise.

Write these BEFORE benchmarking and do not edit them afterwards. Commit them so the
git history shows the date. The gap between this file and the results feeds the
"What surprised you" section.



## Randomised quickselect (Andy)

## Median-of-medians (Joshua)

## std::nth_element baseline

## Which wins where, and where are the crossovers?
