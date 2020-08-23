#ifndef ANOSMELLYA_WORLD_H_
#define ANOSMELLYA_WORLD_H_

#include "Animal.hpp"
#include "Config.hpp"
#include "Grid.hpp"
#include "Random.hpp"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>

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

    Statistics& operator=(Statistics const& copy) = default;

    // Print the information as JSON to the file.
    void print(FILE* to);
};

// Argument for internal fluid dispersal/evaporation worker threads. The worker
// waits on start_sem and the main thread posts when the next fluid tick should
// be calculated. The main thread then waits on stop_sem and the worker posts to
// stop_sem when it is done. The worker loops until the grid pointer is NULL. an
// empty worker thread slot is indicated by a NULL thread pointer.
struct FluidWorker {
    SDL_Thread* thread;
    SDL_sem* start_sem;
    SDL_sem* stop_sem;
    Grid<float>* grid;
    float dispersal;
    float evap;

    FluidWorker& operator=(FluidWorker const& copy) = default;
};

class World {
public:
    World(unsigned width, unsigned height, Random const& random,
        Config const& conf, unsigned max_threads);

    ~World();

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
    std::vector<SDL_Rect> carn_rect_buf;
    std::vector<SDL_Rect> herb_rect_buf;
    std::vector<SDL_Rect> receptive_carn_rect_buf;
    std::vector<SDL_Rect> receptive_herb_rect_buf;
    // Three worker threads means four threads total for the four fluids. The
    // world object can't be moved because workers reference these structs.
    FluidWorker workers[3];
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_WORLD_H_ */
