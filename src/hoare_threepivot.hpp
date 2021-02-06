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
// return_value.fifth = iterator past the end of the third subarray
// return_value.sixth = first element of the fourth subarray
template <typename RandomIt, typename Compare>
inline std::tuple<RandomIt, RandomIt, RandomIt, RandomIt, RandomIt, RandomIt> threepivot_hoare_partition_impl(RandomIt first, RandomIt last, RandomIt tmp_pivot_low, RandomIt tmp_pivot_mid, RandomIt tmp_pivot_high, Compare&& comp) {
    assert(first < last);
    const RandomIt empty_slot = first;
    --last;
    ++first;
    const RandomIt pivot_high = last;
    const RandomIt pivot_low = first;
    RandomIt low_end = first;
    RandomIt high_begin = last;
    typename std::iterator_traits<RandomIt>::value_type pivot_mid;
    {
        if (comp(*tmp_pivot_mid, *tmp_pivot_high)) {
            if (comp(*tmp_pivot_low, *tmp_pivot_mid)) {
                pivot_mid = std::exchange(*tmp_pivot_mid, std::move(*empty_slot));
                std::iter_swap(tmp_pivot_high, pivot_high);
                std::iter_swap(tmp_pivot_low, pivot_low);
            }
            else if (comp(*tmp_pivot_low, *tmp_pivot_high)) {
                pivot_mid = std::exchange(*tmp_pivot_low, std::move(*empty_slot));
                std::iter_swap(tmp_pivot_high, pivot_high);
                std::iter_swap(tmp_pivot_mid, pivot_low);
            }
            else {
                pivot_mid = std::exchange(*tmp_pivot_high, std::move(*empty_slot));
                std::iter_swap(tmp_pivot_low, pivot_high);
                std::iter_swap(tmp_pivot_mid, pivot_low);
            }
        }
        else {
            if (comp(*tmp_pivot_low, *tmp_pivot_high)) {
                pivot_mid = std::exchange(*tmp_pivot_high, std::move(*empty_slot));
                std::iter_swap(tmp_pivot_mid, pivot_high);
                std::iter_swap(tmp_pivot_low, pivot_low);
            }
            else if (comp(*tmp_pivot_low, *tmp_pivot_mid)) {
                pivot_mid = std::exchange(*tmp_pivot_low, std::move(*empty_slot));
                std::iter_swap(tmp_pivot_mid, pivot_high);
                std::iter_swap(tmp_pivot_high, pivot_low);
            }
            else {
                pivot_mid = std::exchange(*tmp_pivot_mid, std::move(*empty_slot));
                std::iter_swap(tmp_pivot_low, pivot_high);
                std::iter_swap(tmp_pivot_high, pivot_low);
            }
        }
    }
    // now the low pivot is at the front position, and the high pivot is at the back position
    // so we can now do non-bounds-checked while-loops

    --last;
    ++first;

    // first: next element on left side to compare
    // last: next element on right side to compare
    // low_end: last element (not past the end!) of the first subarray
    // high_begin: first element of the last subarray
    while (true) {
        while (comp(*first, pivot_mid)) {
            // *first should go into the first or second partition
            if (comp(*first, *pivot_low)) {
                // *first should go into the first partition
                std::iter_swap(++low_end, first);
            }
            ++first;
        }
        while (!comp(*last, pivot_mid)) {
            // *last should go into the third or fourth partition
            if (comp(*pivot_high, *last)) {
                // *last should go into the fourth partition
                std::iter_swap(--high_begin, last);
            }
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
        if (comp(*pivot_high, *last)) {
            // *last should go into the fourth partition
            std::iter_swap(--high_begin, last);
        }
        ++first;
        --last;
    }

    // decrement high_begin to the first element strictly less than *pivot_high (which is now at high_begin)
    RandomIt highmid_end = high_begin;
    std::iter_swap(highmid_end, pivot_high);
    for (--highmid_end; highmid_end != last; --highmid_end) {
        if (comp(*highmid_end, *high_begin)) {
            break;
        }
    }
    ++highmid_end;
    ++high_begin;

    // fill in the empty space
    *empty_slot = std::move(*low_end);
    RandomIt real_low_end = low_end;
    --real_low_end;
    if (pivot_low != low_end) {
        std::iter_swap(pivot_low, real_low_end);
    }
    *low_end = std::move(*last);

    // increment mid_begin to the first element strictly greater than *pivot_low (which is now at real_low_end)
    RandomIt lowmid_begin = low_end;
    for (; lowmid_begin != last; ++lowmid_begin) {
        if (comp(*real_low_end, *lowmid_begin)) {
            break;
        }
    }

    // increment first to the first element strictly greater than pivot_mid
    for (; first != highmid_end; ++first) {
        if (comp(pivot_mid, *first)) {
            break;
        }
    }
    *last = std::move(pivot_mid);

    return { real_low_end, lowmid_begin, last, first, highmid_end, high_begin };
}

// Call this function to drive the whole quicksort
template <size_t InsertionSortThreshold, typename RandomIt, typename Compare, typename TriPivotSelector>
inline void threepivot_hoare_quicksort_impl(RandomIt first, RandomIt last, Compare&& comp, TriPivotSelector&& tripivot_selector) {
    while (last - first > static_cast<std::ptrdiff_t>(InsertionSortThreshold)) {
        // pick the two pivots, ensures that pivot_low != pivot_high
        auto [pivot_low, pivot_mid, pivot_high] = tripivot_selector(first + 2, last - 1, comp);

        // partition the array
        auto [low_end, midlow_begin, midlow_end, midhigh_begin, midhigh_end, high_begin] = threepivot_hoare_partition_impl(first, last, pivot_low, pivot_mid, pivot_high, comp);

        // recurse on all sub-ranges
        threepivot_hoare_quicksort_impl<InsertionSortThreshold>(high_begin, last, comp, tripivot_selector);
        threepivot_hoare_quicksort_impl<InsertionSortThreshold>(midhigh_begin, midhigh_end, comp, tripivot_selector);
        threepivot_hoare_quicksort_impl<InsertionSortThreshold>(midlow_begin, midlow_end, comp, tripivot_selector);
        last = low_end;
    }
}

template <size_t InsertionSortThreshold, typename Compare, typename TriPivotSelector>
class threepivot_hoare_quicksort {
private:
    Compare comp;
    TriPivotSelector tripivot_selector;

public:
    threepivot_hoare_quicksort(Compare comp, TriPivotSelector tripivot_selector)
        : comp(std::move(comp)), tripivot_selector(std::move(tripivot_selector)) {}

    // Call this function to drive the whole Hoare quicksort
    template <typename RandomIt>
    void operator()(RandomIt first, RandomIt last) {
        const auto dist = last - first;
        if (dist >= 2) {
            threepivot_hoare_quicksort_impl<InsertionSortThreshold>(first, last, comp, std::move(tripivot_selector));
            final_insertion_sort<InsertionSortThreshold>(first, last, std::move(comp));
        }
    }
};

template <size_t InsertionSortThreshold, typename Compare, typename TriPivotSelector>
threepivot_hoare_quicksort<InsertionSortThreshold, Compare, TriPivotSelector> make_threepivot_hoare_quicksort(
    Compare comp, TriPivotSelector tripivot_selector) {
    return threepivot_hoare_quicksort<InsertionSortThreshold, Compare, TriPivotSelector>{std::move(comp), std::move(tripivot_selector)};
}
