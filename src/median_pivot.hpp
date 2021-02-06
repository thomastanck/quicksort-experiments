#pragma once

#include <iterator>
#include <tuple>
#include <utility>

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

struct seven_median_tripivot_selector {
    template <typename RandomIt, typename Compare>
    std::tuple<RandomIt, RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, Compare&& comp) const {
        return get_seven_median_tripivot(first, last, comp);
    }
};
