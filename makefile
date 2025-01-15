
test: build/checked_array_tests build/checked_array.o
	./build/checked_array_tests

build/checked_array_tests: build/checked_array_debug.o checked_array_tests.c
	gcc -Og -g -Wall -Werror $^ -o $@

build/checked_array_debug.o: build checked_array.c checked_array.h
	gcc -Og -g -c checked_array.c -Wall -Werror -o $@

build/checked_array.o: build checked_array.c checked_array.h
	gcc -O2 -c checked_array.c -Wall -Werror -o $@

build:
	mkdir build

clean:
	rm -rf build
