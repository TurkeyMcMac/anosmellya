#ifndef ANOSMELLYA_WORLD_H_
#define ANOSMELLYA_WORLD_H_

#include "Animal.hpp"
#include "Grid.hpp"
#include "Random.hpp"
#include <SDL2/SDL.h>

namespace anosmellya {

class World {
public:
    World(unsigned width, unsigned height, Random& random, float herb_chance,
        float carn_chance);

    unsigned get_width();

    unsigned get_height();

    void simulate(Random& random);

    void draw(SDL_Renderer* renderer);

private:
    Grid<Animal> animal;
    Grid<float> plant;
    Grid<float> herb;
    Grid<float> carn;
    Grid<float> baby;
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_WORLD_H_ */
