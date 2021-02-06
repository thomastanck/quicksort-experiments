#pragma once

#include <iterator>
#include <tuple>
#include <utility>

#include "insertion_sort.hpp"

#ifdef _MSC_VER
#   define always_inline __forceinline
#elif defined(__GNUC__)
#   define always_inline __attribute__((always_inline)) inline
#else
#   define always_inline inline
#endif

template <typename RandomIt, typename Compare>
always_inline RandomIt get_median_iterator(RandomIt a, RandomIt b, RandomIt c, Compare&& comp) {
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

template <typename RandomIt, typename Compare>
always_inline std::pair<RandomIt, RandomIt> get_median_iterator(RandomIt a, RandomIt b, RandomIt c, RandomIt d, RandomIt e, Compare&& comp) {
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    using std::swap;
    if (comp(*c, *b)) swap(c, b);
    if (comp(*e, *d)) swap(e, d);
    if (comp(*d, *b)) swap(d, b);
    if (comp(*c, *a)) swap(c, a);
    if (comp(*e, *c)) swap(e, c);
    if (comp(*d, *a)) swap(d, a);
    if (comp(*b, *a)) swap(b, a);
    if (comp(*d, *c)) swap(d, c);
    if (comp(*c, *b)) swap(c, b);
    return { b,d };
}

template <typename RandomIt, typename Compare>
always_inline std::tuple<RandomIt, RandomIt, RandomIt> get_median_iterator(RandomIt a, RandomIt b, RandomIt c, RandomIt d, RandomIt e, RandomIt f, RandomIt g, Compare&& comp) {
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    using std::swap;
    if (comp(*c, *b)) swap(c, b);
    if (comp(*e, *d)) swap(e, d);
    if (comp(*g, *f)) swap(g, f);
    if (comp(*c, *a)) swap(c, a);
    if (comp(*g, *e)) swap(g, e);
    if (comp(*f, *d)) swap(f, d);
    if (comp(*g, *c)) swap(g, c);
    if (comp(*f, *b)) swap(f, b);
    if (comp(*e, *a)) swap(e, a);
    if (comp(*f, *c)) swap(f, c);
    if (comp(*d, *a)) swap(d, a);
    if (comp(*e, *c)) swap(e, c);
    if (comp(*d, *b)) swap(d, b);
    if (comp(*f, *e)) swap(f, e);
    if (comp(*d, *c)) swap(d, c);
    if (comp(*b, *a)) swap(b, a);
    return { b,d,f };
}

template <typename RandomIt, typename Compare>
inline RandomIt get_tri_median_pivot(RandomIt first, RandomIt last, Compare&& comp) {
    const RandomIt b = first + (std::distance(first, last) >> 1);
    const RandomIt a = first;
    const RandomIt c = last - 2;
    return get_median_iterator(a, b, c, std::forward<Compare>(comp));
}

struct tri_median_pivot_selector {
    template <typename RandomIt, typename Compare>
    RandomIt operator()(RandomIt first, RandomIt last, Compare&& comp) const {
        return get_tri_median_pivot(first, last, comp);
    }
};



template <typename RandomIt, typename Compare>
inline std::pair<RandomIt, RandomIt> get_five_median_bipivot(RandomIt first, RandomIt last, Compare&& comp) {
    RandomIt b = first + (std::distance(first, last) >> 1);
    RandomIt a = first;
    RandomIt c = last - 2;
    RandomIt d = a + (std::distance(a, b) >> 1);
    RandomIt e = b + (std::distance(b, c) >> 1);
    return get_median_iterator(a, b, c, d, e, std::forward<Compare>(comp));
}

struct five_median_bipivot_selector {
    template <typename RandomIt, typename Compare>
    std::pair<RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, Compare&& comp) const {
        return get_five_median_bipivot(first, last, comp);
    }
};



template <typename RandomIt, typename Compare>
inline std::tuple<RandomIt, RandomIt, RandomIt> get_seven_median_tripivot(RandomIt first, RandomIt last, Compare&& comp) {
    RandomIt b = first + (std::distance(first, last) >> 1);
    RandomIt a = first;
    RandomIt c = last - 2;
    RandomIt d = a + (std::distance(a, b) >> 1);
    RandomIt e = b + (std::distance(b, c) >> 1);
    RandomIt f = c - 1;
    RandomIt g = a + 1;
    return get_median_iterator(a, b, c, d, e, f, g, std::forward<Compare>(comp));
}

struct seven_median_tripivot_selector {
    template <typename RandomIt, typename Compare>
    std::tuple<RandomIt, RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, Compare&& comp) const {
        return get_seven_median_tripivot(first, last, comp);
    }
};
