#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <string>
#include <vector>

#include "sorters.hpp"

void usage() {
    std::cerr << "Usage: ./sorting_benchmark (int | string) <ID> <num_repeats>\n";
    std::cerr << "ID should be between 0 and "
        << num_sorters - 1
        << ".\n";
}

auto int_sorter_benchmark(int sorter_id, int num_repeats) {
    std::vector<int> dataset;
    {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        std::string line;
        while (std::cin >> line) {
            dataset.push_back(std::stoi(line));
        }
    }

    volatile int clobber = 0;
    std::vector<int> v;
    v.reserve(dataset.size());

    auto start = std::chrono::high_resolution_clock::now();

    apply_sorter(
            sorter_id,
            [&](auto &&sorter) {
                for (int i = 0; i < num_repeats; i++) {
                    v.clear();
                    std::copy(dataset.begin(), dataset.end(), std::back_inserter(v));
                    sorter(v.begin(), v.end());
                    clobber = v[0];
                }
            });

    auto end = std::chrono::high_resolution_clock::now();

    if (!std::is_sorted(v.begin(), v.end())) {
        std::cout << "not sorted correctly\n";
        exit(1);
    }

    return end - start;
}

auto string_sorter_benchmark(int sorter_id, int num_repeats) {
    std::vector<std::string> dataset;
    {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        std::string line;
        while (std::getline(std::cin, line)) {
            dataset.push_back(line);
        }
    }

    volatile int clobber = 0;
    std::vector<std::string> v;
    v.reserve(dataset.size());

    auto start = std::chrono::high_resolution_clock::now();

    apply_sorter(
            sorter_id,
            [&](auto &&sorter) {
                for (int i = 0; i < num_repeats; i++) {
                    v.clear();
                    std::copy(dataset.begin(), dataset.end(), std::back_inserter(v));
                    sorter(v.begin(), v.end());
                    clobber = v[0][0];
                }
            });

    auto end = std::chrono::high_resolution_clock::now();

    if (!std::is_sorted(v.begin(), v.end())) {
        std::cout << "not sorted correctly\n";
        exit(1);
    }

    return end - start;
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

    auto time_taken = is_int
        ? int_sorter_benchmark(sorter_id, num_repeats)
        : string_sorter_benchmark(sorter_id, num_repeats);
    auto time_taken_in_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_taken);

    std::cout << time_taken_in_milliseconds.count() << "\n";

    return 0;
}
