all: unit_tests sorting_benchmark sorting_benchmark_debug

unit_tests: src/unit_tests.cpp src/*.hpp
	g++ --std=c++17 -Wall -g src/unit_tests.cpp -o unit_tests

sorting_benchmark: src/sorting_benchmark.cpp src/*.hpp
	g++ --std=c++17 -Wall -DNDEBUG -O3 src/sorting_benchmark.cpp -o sorting_benchmark

sorting_benchmark_debug: src/sorting_benchmark.cpp src/*.hpp
	g++ --std=c++17 -Wall -g src/sorting_benchmark.cpp -o sorting_benchmark_debug

clean:
	rm unit_tests sorting_benchmark sorting_benchmark_debug
