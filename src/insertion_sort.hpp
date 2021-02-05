#pragma once

#include <algorithm>
#include <iterator>
#include <memory>

#ifndef INSERTION_SORT_THRESHOLD
#define INSERTION_SORT_THRESHOLD (16)
#endif

// Moves *it forward until it is just after an element that is not larger than it.
template <typename RandomIt, typename Compare>
inline void unguarded_linear_insert_impl(RandomIt it, Compare&& comp) {
    typename std::iterator_traits<RandomIt>::value_type tmp = std::move(*it);
    RandomIt it2 = it;
    RandomIt prev = it2;
    --prev;
    while (comp(tmp, *prev)) {
        *it2 = std::move(*prev);
        it2 = prev;
        --prev;
    }
    *it2 = std::move(tmp);
}

template <typename RandomIt, typename Compare>
inline void guarded_insertion_sort_impl(RandomIt first, RandomIt last, Compare&& comp) {
    for (RandomIt it = first + 1; it != last; ++it) {
        if (comp(*it, *first)) {
            // *it is smaller than first element
            // so we move the element to the front and move everything else back by one position
            typename std::iterator_traits<RandomIt>::value_type tmp = std::move(*it);
            std::move_backward(first, it, it + 1);
            *first = std::move(tmp);
        }
        else {
            // first element is at least as small as *it, so we don't need to check if we overrun the beginning
            unguarded_linear_insert_impl(it, std::forward<Compare>(comp));
        }
    }
}

// Expects: Each element is less than INSERTION_SORT_THRESHOLD positions away from its proper location, and last-first>=2
// Ensures: Range is sorted.
template <typename RandomIt, typename Compare>
inline void final_insertion_sort(RandomIt first, RandomIt last, Compare&& comp) {
    assert(first + 2 <= last);
    if (last - first > static_cast<std::ptrdiff_t>(INSERTION_SORT_THRESHOLD)) {
        RandomIt threshold_it = first + INSERTION_SORT_THRESHOLD + 1;
        guarded_insertion_sort_impl(first, threshold_it, comp);
        for (RandomIt it = threshold_it; it != last; ++it) {
            unguarded_linear_insert_impl(it, comp);
        }
    }
    else {
        guarded_insertion_sort_impl(first, last, std::forward<Compare>(comp));
    }
}
