all: unit_tests sorting_benchmark

unit_tests: src/unit_tests.cpp src/*.hpp
	g++ --std=c++17 -Wall src/unit_tests.cpp -o unit_tests

sorting_benchmark: src/sorting_benchmark.cpp src/*.hpp
	g++ --std=c++17 -Wall -DNDEBUG -O3 src/sorting_benchmark.cpp -o sorting_benchmark

clean:
	rm unit_tests sorting_benchmark
