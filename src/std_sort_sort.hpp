#include <algorithm>

struct std_sort_sort {
    template <typename RandomIt>
    void operator()(RandomIt first, RandomIt last) { std::sort(first, last); }
};
