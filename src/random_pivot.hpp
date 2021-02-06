#pragma once

#include <random>
#include <utility>

#include "insertion_sort.hpp"
#include "median_pivot.hpp"

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
struct random_median_pivot_selector {
private:
    URBG& _urbg;
public:
    random_median_pivot_selector(URBG& urbg) noexcept :_urbg(urbg) {}
    template <typename RandomIt, typename Compare>
    RandomIt operator()(RandomIt first, RandomIt last, Compare&& comp) {
        RandomIt it1 = get_random_pivot(first, last, _urbg);
        RandomIt it2 = get_random_pivot(first, last, _urbg);
        RandomIt it3 = get_random_pivot(first, last, _urbg);
        return get_median_iterator(it1, it2, it3, std::forward<Compare>(comp));
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
        RandomIt it2 = get_random_pivot(first, last - 1, _urbg);
        it2 += it2 >= it1;
        return { it1, it2 };
    }
};

template <typename URBG>
struct random_median_twopivot_selector {
private:
    URBG& _urbg;
public:
    random_median_twopivot_selector(URBG& urbg) noexcept :_urbg(urbg) {}
    template <typename RandomIt, typename Compare>
    std::pair<RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, Compare&& comp) {
        RandomIt it1 = get_random_pivot(first, last, _urbg);
        RandomIt it2 = get_random_pivot(first, last - 1, _urbg);
        RandomIt it3 = get_random_pivot(first, last - 2, _urbg);
        RandomIt it4 = get_random_pivot(first, last - 3, _urbg);
        RandomIt it5 = get_random_pivot(first, last - 4, _urbg);
        it5 += it2 >= it1;
        it4 += it2 >= it1;
        it3 += it2 >= it1;
        it2 += it2 >= it1;
        it5 += it3 >= it2;
        it4 += it3 >= it2;
        it3 += it3 >= it2;
        it5 += it4 >= it3;
        it4 += it4 >= it3;
        it5 += it5 >= it4;
        return get_median_iterator(it1, it2, it3, it4, it5, std::forward<Compare>(comp));
    }
};

template <typename URBG>
struct random_threepivot_selector {
private:
    URBG& _urbg;
public:
    random_threepivot_selector(URBG& urbg) noexcept :_urbg(urbg) {}
    template <typename RandomIt, typename Compare>
    std::tuple<RandomIt, RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, Compare&&) {
        // magic to pick two distinct pivots
        RandomIt it1 = get_random_pivot(first, last, _urbg);
        RandomIt it2 = get_random_pivot(first, last - 1, _urbg);
        RandomIt it3 = get_random_pivot(first, last - 2, _urbg);
        it3 += it2 >= it1;
        it2 += it2 >= it1;
        it3 += it3 >= it2;
        return { it1, it2, it3 };
    }
};

template <typename URBG>
struct random_median_threepivot_selector {
private:
    URBG& _urbg;
public:
    random_median_threepivot_selector(URBG& urbg) noexcept :_urbg(urbg) {}
    template <typename RandomIt, typename Compare>
    std::tuple<RandomIt, RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, Compare&& comp) {
        RandomIt it1 = get_random_pivot(first, last, _urbg);
        RandomIt it2 = get_random_pivot(first, last - 1, _urbg);
        RandomIt it3 = get_random_pivot(first, last - 2, _urbg);
        RandomIt it4 = get_random_pivot(first, last - 3, _urbg);
        RandomIt it5 = get_random_pivot(first, last - 4, _urbg);
        RandomIt it6 = get_random_pivot(first, last - 5, _urbg);
        RandomIt it7 = get_random_pivot(first, last - 6, _urbg);
        it7 += it2 >= it1;
        it6 += it2 >= it1;
        it5 += it2 >= it1;
        it4 += it2 >= it1;
        it3 += it2 >= it1;
        it2 += it2 >= it1;
        it7 += it3 >= it2;
        it6 += it3 >= it2;
        it5 += it3 >= it2;
        it4 += it3 >= it2;
        it3 += it3 >= it2;
        it7 += it4 >= it3;
        it6 += it4 >= it3;
        it5 += it4 >= it3;
        it4 += it4 >= it3;
        it7 += it5 >= it4;
        it6 += it5 >= it4;
        it5 += it5 >= it4;
        it7 += it6 >= it5;
        it6 += it6 >= it5;
        it7 += it7 >= it6;
        return get_median_iterator(it1, it2, it3, it4, it5, it6, it7, std::forward<Compare>(comp));
    }
};
