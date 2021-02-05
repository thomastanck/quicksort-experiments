#pragma once

#include <tuple>

#include "insertion_sort.hpp"

// 2-pivot mixed hoare-lomuto

// Partition the array based on the given pivot
// Requires that tmp_pivot_low != tmp_pivot_high and tmp_pivot_low and tmp_pivot_high not first or last-1
// return_value.first = iterator past the end of the first subarray
// return_value.second = first element of the second subarray
// return_value.third = iterator past the end of the second subarray
// return_value.fourth = first element of the third subarray
template <typename RandomIt, typename Compare>
inline std::tuple<RandomIt, RandomIt, RandomIt, RandomIt> twopivot_hoare_partition_impl(RandomIt first, RandomIt last, RandomIt tmp_pivot_low, RandomIt tmp_pivot_high, Compare&& comp) {
    assert(first < last);
    --last;
    const RandomIt pivot_high = last;
    const RandomIt pivot_low = first;
    RandomIt low_end = first;
    {
        intptr_t cmp = comp(*tmp_pivot_low, *tmp_pivot_high);
        --cmp;
        using std::swap;
        swap(*reinterpret_cast<typename std::iterator_traits<RandomIt>::pointer>((~cmp & reinterpret_cast<intptr_t>(&*tmp_pivot_high)) | (cmp & reinterpret_cast<intptr_t>(&*tmp_pivot_low))), *pivot_high);
        swap(*reinterpret_cast<typename std::iterator_traits<RandomIt>::pointer>((~cmp & reinterpret_cast<intptr_t>(&*tmp_pivot_low)) | (cmp & reinterpret_cast<intptr_t>(&*tmp_pivot_high))), *pivot_low);

        // the above does the same thing as below, not sure which is faster
        /*
        const intptr_t cmp = comp(*tmp_pivot_low, *tmp_pivot_high);
        std::iter_swap(cmp ? tmp_pivot_high : tmp_pivot_low, pivot_high);
        std::iter_swap(cmp ? tmp_pivot_low : tmp_pivot_high, pivot_low);
        */
    }
    // now the low pivot is at the front position, and the high pivot is at the back position
    // so we can now do non-bounds-checked while-loops

    --last;
    ++first;

    // first: next element on left side to compare
    // last: next element on right side to compare
    // low_end: last element (not past the end!) of the first subarray
    while (true) {
        while (comp(*first, *pivot_high)) {
            // *first should go into the first or second partition
            if (comp(*first, *pivot_low)) {
                // *first should go into the first partition
                std::iter_swap(++low_end, first);
            }
            ++first;
        }
        while (!comp(*last, *pivot_high)) {
            --last;
        }
        if (first > last)
            break;
        assert(first < last);
        std::iter_swap(first, last);
        if (comp(*first, *pivot_low)) {
            // *first should go into the first partition
            std::iter_swap(++low_end, first);
        }
        ++first;
        --last;
    }
    ++last;

    // increment last to the first element strictly greater than *pivot_high
    for (; last != pivot_high; ++last) {
        if (comp(*pivot_high, *last)) {
            break;
        }
    }
    std::iter_swap(last, pivot_high);
    ++last;

    std::iter_swap(pivot_low, low_end);
    RandomIt mid_begin = low_end;
    ++mid_begin;
    // increment mid_begin to the first element strictly greater than *pivot_low (which is now at low_end)
    for (; mid_begin != first; ++mid_begin) {
        if (comp(*low_end, *mid_begin)) {
            break;
        }
    }

    return { low_end, mid_begin, first, last };
}

// Call this function to drive the whole quicksort
template <size_t InsertionSortThreshold, typename RandomIt, typename Compare, typename BiPivotSelector>
inline void twopivot_hoare_quicksort_impl(RandomIt first, RandomIt last, Compare&& comp, BiPivotSelector&& bipivot_selector) {
    while (last - first > static_cast<std::ptrdiff_t>(InsertionSortThreshold)) {
        // pick the two pivots, ensures that pivot_low != pivot_high
        auto [pivot_low, pivot_high] = bipivot_selector(first + 1, last - 1, comp);

        // partition the array
        auto [low_end, mid_begin, mid_end, high_begin] = twopivot_hoare_partition_impl(first, last, pivot_low, pivot_high, comp);

        // recurse on all sub-ranges
        twopivot_hoare_quicksort_impl<InsertionSortThreshold>(high_begin, last, comp, bipivot_selector);
        twopivot_hoare_quicksort_impl<InsertionSortThreshold>(mid_begin, mid_end, comp, bipivot_selector);
        last = low_end;
    }
}

template <size_t InsertionSortThreshold, typename Compare, typename BiPivotSelector>
class twopivot_hoare_quicksort {
private:
    Compare comp;
    BiPivotSelector bipivot_selector;

public:
    twopivot_hoare_quicksort(Compare comp, BiPivotSelector bipivot_selector)
        : comp(std::move(comp)), bipivot_selector(std::move(bipivot_selector)) {}

    // Call this function to drive the whole Hoare quicksort
    template <typename RandomIt>
    void operator()(RandomIt first, RandomIt last) {
        const auto dist = last - first;
        if (dist >= 2) {
            twopivot_hoare_quicksort_impl<InsertionSortThreshold>(first, last, comp, std::move(bipivot_selector));
            final_insertion_sort<InsertionSortThreshold>(first, last, std::move(comp));
        }
    }
};

template <size_t InsertionSortThreshold, typename Compare, typename BiPivotSelector>
twopivot_hoare_quicksort<InsertionSortThreshold, Compare, BiPivotSelector> make_twopivot_hoare_quicksort(
    Compare comp, BiPivotSelector bipivot_selector) {
    return twopivot_hoare_quicksort<InsertionSortThreshold, Compare, BiPivotSelector>{std::move(comp), std::move(bipivot_selector)};
}
