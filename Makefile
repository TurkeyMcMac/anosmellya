target = anosmellya
flags = `sdl2-config --cflags` -O3 -flto -Wall -Wextra -Wpedantic -std=c++11 \
	-DVERSION=\"`cat version`\" $(CXXFLAGS)
libs = `sdl2-config --libs`

CXX ?= c++

$(target): src/* version
	$(CXX) $(flags) -o $@ src/*.cpp $(libs)

.PHONY: fmt
fmt:
	clang-format -i src/*

.PHONY: clean
clean:
	rm -f $(target)
