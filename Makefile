flags = -O3 -flto -Wall -Wextra -std=c++11 -DVERSION=\"`cat version`\"

CXX ?= c++

anosmellya: src/* version
	$(CXX) $(flags) -o $@ src/*.cpp -lSDL2

.PHONY: fmt
fmt:
	clang-format -i src/*

.PHONY: clean
clean:
	rm -f anosmellya
