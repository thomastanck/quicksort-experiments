#include "hoare.hpp"
#include "hoare_twopivot.hpp"
#include "hoare_threepivot.hpp"
#include "insertion_sort.hpp"
#include "measure.hpp"
#include "median_pivot.hpp"
#include "random_pivot.hpp"
#include "std_sort_sort.hpp"
#include "xorshift.hpp"

static std::mt19937_64 mt_rng;
static xorshift64 xs_rng;

static auto sorters =
    std::tuple(
            std_sort_sort{},
            make_hoare_quicksort<16>(std::less<>{}, tri_median_pivot_selector{}),
            make_hoare_quicksort<16>(std::less<>{}, random_pivot_selector{ xs_rng }),
            make_twopivot_hoare_quicksort<16>(std::less<>{}, five_median_bipivot_selector{}),
            make_twopivot_hoare_quicksort<16>(std::less<>{}, random_twopivot_selector{ xs_rng }),
            make_threepivot_hoare_quicksort<16>(std::less<>{}, five_median_tripivot_selector{}),
            make_threepivot_hoare_quicksort<16>(std::less<>{}, random_threepivot_selector{ xs_rng }),

            make_hoare_quicksort<16>(std::less<>{}, tri_median_pivot_selector{}),
            make_hoare_quicksort<32>(std::less<>{}, tri_median_pivot_selector{}),
            make_hoare_quicksort<16>(std::less<>{}, random_pivot_selector{ mt_rng }),
            make_hoare_quicksort<32>(std::less<>{}, random_pivot_selector{ mt_rng }),
            make_hoare_quicksort<16>(std::less<>{}, random_pivot_selector{ xs_rng }),
            make_hoare_quicksort<32>(std::less<>{}, random_pivot_selector{ xs_rng }));

static constexpr auto num_sorters = std::tuple_size_v<decltype(sorters)>;

/**
 * Calls func on the given sorter id.
 */
template <int i = 0, typename Func>
static void apply_sorter(int sorter_id, Func func) {
    if constexpr (i < num_sorters) {
        if (sorter_id == i) {
            func(std::get<i>(sorters));
        } else {
            apply_sorter<i+1>(sorter_id, func);
        }
    }
}


