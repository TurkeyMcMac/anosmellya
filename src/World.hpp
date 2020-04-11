#ifndef ANOSMELLYA_WORLD_H_
#define ANOSMELLYA_WORLD_H_

#include "Animal.hpp"
#include "Config.hpp"
#include "Grid.hpp"
#include "Random.hpp"
#include <SDL2/SDL.h>
#include <stdio.h>

namespace anosmellya {

struct Statistics {
    Animal herb_avg;
    unsigned herb_count;
    Animal carn_avg;
    unsigned carn_count;
    float plant_total;
    float herb_total;
    float carn_total;
    float baby_total;

    void print(FILE* to);
};

class World {
public:
    World(unsigned width, unsigned height, Random const& random,
        Config const& conf);

    unsigned get_width();

    unsigned get_height();

    void simulate();

    void draw(SDL_Renderer* renderer);

    void get_statistics(Statistics& stats);

private:
    Random random;
    Config conf;
    Grid<Animal> animal;
    Grid<float> plant;
    Grid<float> herb;
    Grid<float> carn;
    Grid<float> baby;
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_WORLD_H_ */
