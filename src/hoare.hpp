#pragma once

#include <utility>
#include <cassert>

#include "insertion_sort.hpp"

/**
Hoare quicksort with one partition
*/

// Partition the array based on the given pivot
// All elements strictly less than the pivot will come before other elements
// return_value.first = iterator past the end of the strictly less elements
// return_value.second = first element starting from return_value.first that is not equal to pivot
template <typename RandomIt, typename Compare>
inline std::pair<RandomIt, RandomIt> hoare_partition_impl(RandomIt first, RandomIt last, RandomIt tmp_pivot, Compare&& comp) {
    assert(first < last);
    --last;
    const RandomIt pivot = last;
    --last;
    std::iter_swap(tmp_pivot, pivot);
    // now pivot is at the back position

    // find an element strictly smaller than pivot, to place at the front position
    for (; last != first; --last) {
        if (comp(*last, *pivot))
            break;
    }
    if (last != first) {
        // can now do non-bounds-checked while-loops
        std::iter_swap(first, last);
        ++first;
        while (true) {
            while (comp(*first, *pivot)) {
                ++first;
            }
            while (!comp(*last, *pivot)) {
                --last;
            }
            if (first > last)
                break;
            assert(first < last);
            std::iter_swap(first, last);
            //assert(comp(*first, *pivot));
            //assert(!comp(*last, *pivot));
            ++first;
            --last;
        }
        ++last;
    }
    // now *first >= *pivot and *pivot > *(last-1)
    // increment last to the first element strictly greater than *pivot
    for (; last != pivot; ++last) {
        if (comp(*pivot, *last)) {
            break;
        }
    }
    std::iter_swap(last, pivot);
    ++last;
    return { first, last };
}

template <size_t InsertionSortThreshold, typename RandomIt, typename Compare, typename PivotSelector>
inline void hoare_quicksort_impl(RandomIt first, RandomIt last, Compare&& comp, PivotSelector&& pivot_selector) {
    while (last - first > static_cast<std::ptrdiff_t>(InsertionSortThreshold)) {
        // pick a pivot
        auto pivot = pivot_selector(first, last, comp);

        // partition the array
        auto [partition_it_lower, partition_it_higher] = hoare_partition_impl(
                first, last, pivot, std::forward<Compare>(comp));

        // recurse on both sub-ranges
        hoare_quicksort_impl<InsertionSortThreshold>(
                partition_it_higher, last, std::forward<Compare>(comp), std::forward<PivotSelector>(pivot_selector));
        last = partition_it_lower;
    }
}

template <size_t InsertionSortThreshold, typename Compare, typename PivotSelector>
class hoare_quicksort {
private:
    Compare comp;
    PivotSelector pivot_selector;

public:
    hoare_quicksort(Compare comp, PivotSelector pivot_selector)
        : comp(std::move(comp)), pivot_selector(std::move(pivot_selector)) {}

    // Call this function to drive the whole Hoare quicksort
    template <typename RandomIt>
    void operator()(RandomIt first, RandomIt last) {
        const auto dist = last - first;
        if (dist >= 2) {
            hoare_quicksort_impl<InsertionSortThreshold>(first, last, comp, std::move(pivot_selector));
            final_insertion_sort<InsertionSortThreshold>(first, last, std::move(comp));
        }
    }
};

template <size_t InsertionSortThreshold, typename Compare, typename PivotSelector>
hoare_quicksort<InsertionSortThreshold, Compare, PivotSelector> make_hoare_quicksort(
        Compare comp, PivotSelector pivot_selector) {
    return hoare_quicksort<InsertionSortThreshold, Compare, PivotSelector>{std::move(comp), std::move(pivot_selector)};
}
