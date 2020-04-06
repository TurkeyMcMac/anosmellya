anosmellya: *.[ch]pp
	c++ -O3 -flto -Wall -Wextra -std=c++11 -o $@ *.cpp -lSDL2
