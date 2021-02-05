#pragma once

#include <random>
#include <utility>

#include "insertion_sort.hpp"

template <typename RandomIt, typename URBG>
inline RandomIt get_random_pivot(RandomIt first, RandomIt last, URBG&& urbg) {
    assert(first < last);
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

template <typename URBG>
struct random_twopivot_selector {
private:
    URBG& _urbg;
public:
    random_twopivot_selector(URBG& urbg) noexcept :_urbg(urbg) {}
    template <typename RandomIt, typename Compare>
    std::pair<RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, Compare&&) {
        // magic to pick two distinct pivots
        RandomIt it1 = get_random_pivot(first, last, _urbg);
        RandomIt it2 = get_random_pivot(first, last-1, _urbg);
        it2 += it2 >= it1;
        return { it1, it2 };
    }
};
