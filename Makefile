all: unit_tests

unit_tests: src/unit_tests.cpp src/*.hpp
	g++ --std=c++17 -Wall -DNDEBUG src/unit_tests.cpp -o unit_tests

clean:
	rm unit_tests
