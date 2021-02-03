#pragma once

#include <random>

#include "insertion_sort.hpp"

template <typename RandomIt, typename URBG>
inline RandomIt get_random_pivot(RandomIt first, RandomIt last, URBG&& urbg) {
    assert(last - first > static_cast<std::ptrdiff_t>(INSERTION_SORT_THRESHOLD));
    return first + std::uniform_int_distribution<std::ptrdiff_t>(0, last - first - 1)(std::forward<URBG>(urbg));
}

template <typename URBG>
struct random_pivot_selector {
private:
    URBG& _urbg;
public:
    random_pivot_selector(URBG& urbg) noexcept :_urbg(urbg) {}
    template <typename RandomIt, typename Compare>
    RandomIt operator()(RandomIt first, RandomIt last, Compare&&) {
        return get_random_pivot(first, last, _urbg);
    }
};