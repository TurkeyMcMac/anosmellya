anosmellya: src/*
	c++ -O3 -flto -Wall -Wextra -std=c++11 -o $@ src/*.cpp -lSDL2
