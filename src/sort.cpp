#include <cassert>
#include <random>
#include <vector>

#include "insertion_sort.hpp"
#include "measure.hpp"
#include "hoare.hpp"
#include "median_pivot.hpp"
#include "random_pivot.hpp"

int main() {
	// some tests
	{
		std::vector sorted{ 1,2,3,4,5 };
		{
			std::vector v{ 1,2,3,4,5 };
			final_insertion_sort(v.begin(), v.end(), std::less<>{});
			assert(v == sorted);
		}
		{
			std::vector v{ 5,4,3,2,1 };
			final_insertion_sort(v.begin(), v.end(), std::less<>{});
			assert(v == sorted);
		}
	}
	{
		std::vector sorted{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };
		{
			std::vector v{ 11,12,13,14,15,16,17,18,19,20,1,2,3,4,5,6,7,8,9,10 };
			final_insertion_sort(v.begin(), v.end(), std::less<>{});
			assert(v == sorted);
		}
	}
	{
		counter move_ct, swap_ct;
		wrapper<int> i1(move_ct, swap_ct), i2(move_ct, swap_ct);
		using std::swap;
		auto tmp = std::move(i1);
		swap(tmp, i2);
		i1 = std::move(tmp);
		tmp = std::move(i1);
		i1 = std::move(tmp);
		swap(i1, i2);
		assert(move_ct.value() == 4);
		assert(swap_ct.value() == 2);
	}
	{
		counter ct;
		auto lambda = [](auto x, auto y) { return x < y; };
		counted_invocable ci(ct, lambda);
		ci(1, 2);
		ci(3, 4);
		ci(6, 5);
		ci("abc", "defg");
		assert(ct.value() == 4);
	}
	{
		std::vector sorted{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };
		{
			std::vector v{ 11,12,13,14,15,16,17,18,19,20,1,2,3,4,5,6,7,8,9,10 };
			hoare_quicksort(v.begin(), v.end(), std::less<>{}, tri_median_pivot_selector{});
			assert(v == sorted);
		}
		{
			std::vector v{ 20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1 };
			hoare_quicksort(v.begin(), v.end(), std::less<>{}, tri_median_pivot_selector{});
			assert(v == sorted);
		}
		std::mt19937_64 mt_rng;
		{
			std::vector v{ 11,12,13,14,15,16,17,18,19,20,1,2,3,4,5,6,7,8,9,10 };
			hoare_quicksort(v.begin(), v.end(), std::less<>{}, random_pivot_selector{ mt_rng });
			assert(v == sorted);
		}
		{
			std::vector v{ 20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1 };
			hoare_quicksort(v.begin(), v.end(), std::less<>{}, random_pivot_selector{ mt_rng });
			assert(v == sorted);
		}
	}
}