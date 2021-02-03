#include <cassert>
#include <vector>

#include "insertion_sort.hpp"

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
}