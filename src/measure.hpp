#pragma once

#include <utility>
#include <stdexcept>

struct counter {
private:
	size_t _count;
public:
	counter() noexcept : _count(0) {}
	counter(const counter&) = delete;
	counter& operator=(const counter&) = delete;
	void increment() noexcept { ++_count; }
	size_t value() const noexcept { return _count; }
	void reset() noexcept { _count = 0; }
};

template <typename T>
struct wrapper {
private:
	counter* _move_ct; // the referenced counter, null if this wrapper got moved from
	counter* _swap_ct; // the referenced counter, null if this wrapper got moved from
	T _t;
public:
	wrapper() noexcept : _move_ct(nullptr), _swap_ct(nullptr) {}
	explicit wrapper(counter& move_ct, counter& swap_ct) : _move_ct(&move_ct), _swap_ct(&swap_ct) {}
	wrapper(counter& move_ct, counter& swap_ct, const T& t) : _move_ct(&move_ct), _swap_ct(&swap_ct), _t(t) {}
	wrapper(counter& move_ct, counter& swap_ct, T&& t) : _move_ct(&move_ct), _swap_ct(&swap_ct), _t(std::move<T>(t)) {}
	wrapper(const wrapper&) = delete;
	wrapper& operator=(const wrapper&) = delete;
	wrapper(wrapper&& other) noexcept :
		_move_ct(std::exchange(other._move_ct, nullptr)),
		_swap_ct(std::exchange(other._swap_ct, nullptr)),
		_t(std::move(other._t)) {
		_move_ct->increment();
	};
	wrapper& operator=(wrapper&& other) noexcept {
		_move_ct = std::exchange(other._move_ct, nullptr);
		_swap_ct = std::exchange(other._swap_ct, nullptr);
		_t = std::move(other._t);
		_move_ct->increment();
		return *this;
	};
	operator T& () noexcept { return _t; }
	operator const T& () const noexcept { return _t; }
	friend void swap(wrapper& a, wrapper& b) {
		if (a._swap_ct != b._swap_ct) throw std::logic_error("Cannot swap wrapper with another wrapper that has a different swap counter!");
		a._swap_ct->increment();
		using std::swap;
		swap(a._move_ct, b._move_ct);
		swap(a._t, b._t);
	}
};

template <typename Callable>
struct counted_invocable {
private:
	counter* _ct;
	Callable _callable;
public:
	counted_invocable(counter& ct, Callable callable) noexcept : _ct(&ct), _callable(std::move(callable)) {}
	counted_invocable(const counted_invocable& other) = delete;
	counted_invocable operator=(const counted_invocable& other) = delete;
	counted_invocable(counted_invocable&& other) noexcept : _ct(std::exchange(other._ct, nullptr)), _callable(std::move(other._callable)) {}
	counted_invocable operator=(counted_invocable&& other) noexcept {
		_ct = std::exchange(other._ct, nullptr);
		_callable = std::move(other._callable);
	}
	template <typename... Args>
	auto operator()(Args&&... args) const noexcept(noexcept(_callable(std::forward<Args>(args)...))) {
		_ct->increment();
		return _callable(std::forward<Args>(args)...);
	}
};