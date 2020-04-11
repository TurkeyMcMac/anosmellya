anosmellya: src/* src/.version.hpp
	c++ -O3 -flto -Wall -Wextra -std=c++11 -o $@ src/*.cpp -lSDL2

src/.version.hpp: version
	echo "#define VERSION \"`cat $<`\"" > $@

.PHONY: fmt
fmt:
	clang-format -i src/*
