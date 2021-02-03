#pragma once

#include <cstdint>
#include <limits>

struct xorshift64 {
public:
	using result_type = uint64_t;
private:
	result_type _state;
public:
	xorshift64(result_type seed = 1234567891011121314ull) noexcept : _state(seed) {}
	constexpr static result_type min() noexcept { return std::numeric_limits<result_type>::min(); }
	constexpr static result_type max() noexcept { return std::numeric_limits<result_type>::max(); }
	result_type operator()() noexcept {
		result_type x = _state;
		x ^= x << 13;
		x ^= x >> 7;
		x ^= x << 17;
		return _state = x;
	}
};
