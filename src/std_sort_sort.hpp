#include <algorithm>

struct std_sort_sort {
    template <typename RandomIt, typename Compare>
    void operator()(RandomIt first, RandomIt last, Compare comp) { std::sort(first, last, comp); }
};
