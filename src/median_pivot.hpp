#pragma once

#include <iterator>

#include "insertion_sort.hpp"

template <typename RandomIt, typename Compare>
inline RandomIt get_tri_median_pivot(RandomIt first, RandomIt last, Compare&& comp) {
    const RandomIt b = first + (std::distance(first, last) >> 1);
    const RandomIt a = first;
    const RandomIt c = last - 2;
    if (comp(*b, *c)) {
        if (comp(*a, *b)) {
            return b;
        }
        else if (comp(*a, *c)) {
            return a;
        }
        else {
            return c;
        }
    }
    else {
        if (comp(*a, *c)) {
            return c;
        }
        else if (comp(*a, *b)) {
            return a;
        }
        else {
            return b;
        }
    }
}

struct tri_median_pivot_selector {
    template <typename RandomIt, typename Compare>
    RandomIt operator()(RandomIt first, RandomIt last, Compare&& comp) {
        return get_tri_median_pivot(first, last, comp);
    }
};
