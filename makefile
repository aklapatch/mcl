
test: build/checked_array_tests
	./build/checked_array_tests

build/checked_array_tests: build/checked_array.o checked_array_tests.c
	gcc $^ -o $@

build/checked_array.o: build checked_array.c checked_array.h
	gcc -c checked_array.c -Wall -Werror -o $@

build:
	mkdir build

clean:
	rm -rf build
