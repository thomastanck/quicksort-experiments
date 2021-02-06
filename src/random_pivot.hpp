#pragma once

#include <iterator>
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

        // more magic to do this branch free

        intptr_t cmp = it2 >= it1;
        it2 += cmp;
        cmp--;
        // cmp = 0x00... if it1 < it2, else cmp = 0xff...
        // if it1 < it2, we do:
        //      it3 \in [first, it1) no incr,
        //      it3 \in [it1, it2-1) incr once,
        //      it3 \in [it2-1, last-2) incr twice
        auto it1p = reinterpret_cast<intptr_t>(&*it1);
        auto it2p = reinterpret_cast<intptr_t>(&*it2);
        auto it3p = reinterpret_cast<intptr_t>(&*it3);
        auto it3p1 = reinterpret_cast<intptr_t>(&*(it3 + 1));
        it3 += (it3p >= ((~cmp & it1p) | (cmp & it2p))) + // first incr
               (it3p1 >= ((~cmp & it2p) | (cmp & it1p))); // second incr

        // The code above should do the same thing as the code below

        /*
        it2 += it2 >= it1;
        if (it1 < it2) {
            it3 += it3 >= it1;
            it3 += it3 >= it2;
        }
        else {
            it3 += it3 >= it2;
            it3 += it3 >= it1;
        }
        */

        assert(it1 != it2);
        assert(it1 != it3);
        assert(it2 != it3);
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
