anosmellya: src/* version
	c++ -O3 -flto -Wall -Wextra -std=c++11 -DVERSION=\"`cat version`\" -o $@ src/*.cpp -lSDL2

.PHONY: fmt
fmt:
	clang-format -i src/*
