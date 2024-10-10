all:
	g++ main.cpp -O3 -std=c++20 -march=native

run: all
	./a.out
