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
    unsigned world_width;
    unsigned world_height;
    Animal herb_avg;
    unsigned herb_count;
    Animal carn_avg;
    unsigned carn_count;
    float plant_total;
    float herb_total;
    float carn_total;
    float baby_total;

    // Print the information as JSON to the file.
    void print(FILE* to);
};

class World {
public:
    World(unsigned width, unsigned height, Random const& random,
        Config const& conf);

    unsigned get_width();

    unsigned get_height();

    // Simulate one tick.
    void simulate();

    void draw_smells(SDL_Renderer* renderer);

    void draw_affs(SDL_Renderer* renderer);

    void draw_animals(SDL_Renderer* renderer);

    // Collect statistics and put them into the stats struct.
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
