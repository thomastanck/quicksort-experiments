#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <string>
#include <vector>

#include "measure.hpp"
#include "sorters.hpp"

static void clobber() {
#if defined(__GNUC__)
  asm volatile("" : : : "memory");
#endif
}

void usage() {
    std::cerr << "Usage: ./sorting_benchmark (int | string) <ID> <num_repeats>\n";
    std::cerr << "ID should be between 0 and "
        << num_sorters - 1
        << ".\n";
}

template <typename T>
auto measure_sorter_runtime(int sorter_id, int num_repeats, std::vector<T> dataset) {
    std::vector<T> v;
    v.reserve(dataset.size());

    auto start = std::chrono::high_resolution_clock::now();

    apply_sorter(
            sorter_id,
            [&](auto &&sorter) {
                for (int i = 0; i < num_repeats; i++) {
                    v.clear();
                    std::copy(dataset.begin(), dataset.end(), std::back_inserter(v));
                    sorter(v.begin(), v.end(), std::less<>{});
                    clobber();
                }
            });

    auto end = std::chrono::high_resolution_clock::now();

    if (!std::is_sorted(v.begin(), v.end())) {
        std::cout << "not sorted correctly\n";
        exit(1);
    }

    return end - start;
}

template <typename T>
auto measure_sorter_perfcounts(int sorter_id, int num_repeats, std::vector<T> dataset) {
    counter move_ct, swap_ct, comp_ct;

    counted_invocable counter_comp(comp_ct, std::less<T>{});

    std::vector<wrapper<T>> v;
    v.reserve(dataset.size());

    for (const T &val : dataset) {
        // Construct and set all the counter wrappers to values in dataset
        v.emplace_back(move_ct, swap_ct, val);
    }

    apply_sorter(
            sorter_id,
            [&](auto &&sorter) { sorter(v.begin(), v.end(), counter_comp); }
            );

    return std::tuple(move_ct.value(), swap_ct.value(), comp_ct.value());
}

auto int_sorter_benchmark(int sorter_id, int num_repeats) {
    // Read int dataset
    std::vector<int> dataset;
    {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        std::string line;
        while (std::cin >> line) {
            dataset.push_back(std::stoi(line));
        }
    }

    auto sorter_runtime = measure_sorter_runtime(sorter_id, num_repeats, dataset);
    auto [move_count, swap_count, comp_count] = measure_sorter_perfcounts(sorter_id, num_repeats, dataset);

    return std::tuple(sorter_runtime, move_count, swap_count, comp_count);
}

auto string_sorter_benchmark(int sorter_id, int num_repeats) {
    // Read string dataset
    std::vector<std::string> dataset;
    {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        std::string line;
        while (std::getline(std::cin, line)) {
            dataset.push_back(line);
        }
    }

    auto sorter_runtime = measure_sorter_runtime(sorter_id, num_repeats, dataset);
    auto [move_count, swap_count, comp_count] = measure_sorter_perfcounts(sorter_id, num_repeats, dataset);

    return std::tuple(sorter_runtime, move_count, swap_count, comp_count);
}

int main(int argc, char *argv[]) {
    if (argc <= 3) {
        usage();
        return 1;
    }

    std::string int_or_string{argv[1]};
    if (int_or_string != "int" && int_or_string != "string") {
        usage();
        return 1;
    }
    bool is_int = int_or_string == "int";
    int sorter_id = std::stoi(argv[2]);
    int num_repeats = std::stoi(argv[3]);

    auto [time_taken, move_count, swap_count, comp_count] =
        is_int
        ? int_sorter_benchmark(sorter_id, num_repeats)
        : string_sorter_benchmark(sorter_id, num_repeats);

    auto time_taken_in_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_taken);

    std::cout
        << time_taken_in_milliseconds.count() << ' '
        << move_count << ' '
        << swap_count << ' '
        << comp_count << '\n';

    return 0;
}
