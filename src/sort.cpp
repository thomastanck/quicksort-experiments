#include <cassert>
#include <vector>

#include "insertion_sort.hpp"
#include "measure.hpp"

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
}