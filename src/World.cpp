#include "World.hpp"
#include <cmath>

using namespace anosmellya;

World::World(
    unsigned width, unsigned height, Random const& random, Config const& conf)
    : random(random)
    , conf(conf)
    , animal(width, height)
    , plant(width, height, 0.)
    , herb(width, height, 0.)
    , carn(width, height, 0.)
    , baby(width, height, 0.)
{
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            Animal an;
            if (conf.initial_animal_chance > this->random.generate(1.)) {
                if (conf.initial_carn_chance > this->random.generate(1.)) {
                    an.be_carn();
                } else {
                    an.be_herb();
                }
                an.pos = Vec2D(x + 0.5, y + 0.5);
                an.mutate(this->random, conf.initial_variation);
            }
            animal.at(x, y) = an;
        }
    }
}

unsigned World::get_width() { return animal.get_width(); }

unsigned World::get_height() { return animal.get_height(); }

static float flow(float a, float b, float portion) { return (b - a) * portion; }

static void disperse(Grid<float>& grid, float portion)
{
    // This flow is not completely symmetrical, but it's good enough.
    for (unsigned y = 0; y < grid.get_height(); ++y) {
        for (unsigned x = 0; x < grid.get_width(); ++x) {
            float& here = grid.at(x, y);
            float& right = grid.at_small_trans(x, y, 1, 0);
            float& below = grid.at_small_trans(x, y, 0, 1);
            float flow_right = flow(here, right, portion);
            float flow_below = flow(here, below, portion);
            here += flow_right;
            right -= flow_right;
            here += flow_below;
            below -= flow_below;
        }
    }
}

static void evaporate(Grid<float>& grid, float portion)
{
    float keep = 1. - portion;
    for (unsigned y = 0; y < grid.get_height(); ++y) {
        for (unsigned x = 0; x < grid.get_width(); ++x) {
            grid.at(x, y) *= keep;
        }
    }
}

static void wrap(float& x, unsigned window)
{
    x = fmod(x, window);
    if (x < 0) {
        x += window;
    }
}

static Vec2D get_smell(Grid<float>& grid, unsigned x, unsigned y)
{
    float right = grid.at_small_trans(x, y, 1, 0);
    float above = grid.at_small_trans(x, y, 0, -1);
    float left = grid.at_small_trans(x, y, -1, 0);
    float below = grid.at_small_trans(x, y, 0, 1);
    float horizontal = right - left;
    float vertical = below - above;
    return Vec2D(horizontal, vertical);
}

static void add_output_impulse(Vec2D& acc, Vec2D input,
    SmellAffinity const& aff, float plant, float carn, float herb, float baby,
    float food)
{
    float in_hypot = hypot(input.x, input.y);
    float in_cos = input.x / in_hypot;
    float in_sin = input.y / in_hypot;
    float out_x = in_cos * aff.impulse.x - in_sin * aff.impulse.y;
    float out_y = in_sin * aff.impulse.x + in_cos * aff.impulse.y;
    float out_hypot = hypot(out_x, out_y);
    float offset = plant * aff.plant_effect + carn * aff.carn_effect
        + herb * aff.herb_effect + baby * aff.baby_effect
        + food * aff.food_effect;
    float scalar = 1. + offset / out_hypot;
    if (isfinite(scalar)) {
        acc.x += out_x * scalar;
        acc.y += out_y * scalar;
    }
}

static bool is_receptive(Animal const& an)
{
    return an.food >= an.baby_threshold;
}

static bool find_empty_space(Grid<Animal>& animal, unsigned& x, unsigned& y)
{
    unsigned tx = x;
    unsigned ty = y;
    animal.small_trans(tx, ty, 1, 0);
    if (!animal.at(tx, ty).is_present) {
        goto found;
    }
    animal.small_trans(tx, ty, -1, -1);
    if (!animal.at(tx, ty).is_present) {
        goto found;
    }
    animal.small_trans(tx, ty, -1, 1);
    if (!animal.at(tx, ty).is_present) {
        goto found;
    }
    animal.small_trans(tx, ty, 1, 1);
    if (!animal.at(tx, ty).is_present) {
    found:
        x = tx;
        y = ty;
        return true;
    }
    return false;
}

static void make_baby(Random& random, Config const& conf, Grid<Animal>& animal,
    unsigned mom_x, unsigned mom_y, Animal& dad)
{
    unsigned kid_x = mom_x;
    unsigned kid_y = mom_y;
    if (find_empty_space(animal, kid_x, kid_y)) {
        Animal& mom = animal.at(mom_x, mom_y);
        Animal kid(random, mom, dad);
        mom.food -= kid.food;
        if (conf.mutate_chance > random.generate(1.)) {
            kid.mutate(random, conf.mutate_amount);
        }
        kid.pos = Vec2D(kid_x + 0.5, kid_y + 0.5);
        kid.just_moved = kid_y > mom_y || kid_x > mom_x;
        animal.at(kid_x, kid_y) = kid;
    }
}

static void tick_animal(Random& random, Config const& conf, unsigned x,
    unsigned y, Grid<Animal>& animal, Grid<float>& plant, Grid<float>& carn,
    Grid<float>& herb, Grid<float>& baby)
{
    Animal& an = animal.at(x, y);
    if (!an.is_present) {
        return;
    }
    if (an.just_moved) {
        an.just_moved = false;
        return;
    }
    ++an.age;
    --an.food;
    if (an.age >= conf.lifespan || !(an.food >= 0.)) {
        an.is_present = false;
        return;
    }
    Vec2D pos_orig = an.pos;
    Vec2D vel_orig = an.vel;
    Vec2D acc(0., 0.);
    float plant_here = plant.at(x, y);
    float carn_here = carn.at(x, y);
    float herb_here = herb.at(x, y);
    float baby_here = baby.at(x, y);
    add_output_impulse(acc, get_smell(plant, x, y), an.plant_aff, plant_here,
        carn_here, herb_here, baby_here, an.food);
    add_output_impulse(acc, get_smell(carn, x, y), an.carn_aff, plant_here,
        carn_here, herb_here, baby_here, an.food);
    add_output_impulse(acc, get_smell(herb, x, y), an.herb_aff, plant_here,
        carn_here, herb_here, baby_here, an.food);
    add_output_impulse(acc, get_smell(baby, x, y), an.baby_aff, plant_here,
        carn_here, herb_here, baby_here, an.food);
    add_output_impulse(acc, an.vel, an.vel_aff, plant_here, carn_here,
        herb_here, baby_here, an.food);
    float acc_divisor = hypot(acc.x, acc.y);
    if (acc_divisor != 0.) {
        an.vel.x += acc.x / acc_divisor * conf.acceleration;
        an.vel.y += acc.y / acc_divisor * conf.acceleration;
    }
    an.vel.x *= 1. - conf.friction;
    an.vel.y *= 1. - conf.friction;
    an.pos.x += an.vel.x;
    an.pos.y += an.vel.y;
    wrap(an.pos.x, animal.get_width());
    wrap(an.pos.y, animal.get_height());
    unsigned tx = an.pos.x;
    unsigned ty = an.pos.y;
    if (is_receptive(an)) {
        baby.at(tx, ty) += an.baby_smell_amount;
    }
    if (an.is_carn) {
        carn.at(tx, ty) += conf.carn_amount;
    } else {
        float eat = plant.at(tx, ty) * conf.herb_eat_portion;
        an.food += eat;
        plant.at(tx, ty) -= eat;
        herb.at(tx, ty) += conf.herb_amount;
    }
    if (tx != x || ty != y) {
        Animal& target = animal.at(tx, ty);
        if (target.is_present) {
            if (an.is_carn && !target.is_carn) {
                float eat = target.food * conf.carn_eat_portion;
                an.food += eat;
                target.food -= eat;
            } else if (!an.is_carn && target.is_carn) {
                float eat = an.food * conf.carn_eat_portion;
                target.food += eat;
                an.food -= eat;
            } else {
                if (is_receptive(target)) {
                    make_baby(random, conf, animal, tx, ty, an);
                } else if (is_receptive(an)) {
                    make_baby(random, conf, animal, x, y, target);
                }
            }
            an.pos = pos_orig;
            an.vel = vel_orig;
        } else {
            target = an;
            target.just_moved = ty > y || tx > x;
            an.is_present = false;
        }
    }
}

void World::simulate()
{
    disperse(plant, conf.plant_dispersal);
    evaporate(plant, conf.plant_evap);
    disperse(herb, conf.herb_dispersal);
    evaporate(herb, conf.herb_evap);
    disperse(carn, conf.carn_dispersal);
    evaporate(carn, conf.carn_evap);
    disperse(baby, conf.baby_dispersal);
    evaporate(baby, conf.baby_evap);
    for (unsigned y = 0; y < get_height(); ++y) {
        for (unsigned x = 0; x < get_width(); ++x) {
            tick_animal(random, conf, x, y, animal, plant, carn, herb, baby);
        }
    }
    for (unsigned i = 0;
         i < get_width() * get_height() * conf.plant_place_chance
             + random.generate(1.);
         ++i) {
        plant.at(
            random.generate() % get_width(), random.generate() % get_height())
            += conf.plant_place_amount;
    }
}

static uint8_t amount2color(float amount)
{
    amount = fabs(amount * 3.);
    if (amount > 128.) {
        return 128;
    } else {
        return (uint8_t)amount;
    }
}

void World::draw(SDL_Renderer* renderer)
{
    SDL_Rect viewport;
    SDL_RenderGetViewport(renderer, &viewport);
    SDL_Rect tile;
    tile.w = viewport.w / get_width();
    tile.h = viewport.h / get_height();
    for (unsigned y = 0; y < get_height(); ++y) {
        for (unsigned x = 0; x < get_width(); ++x) {
            tile.x = (int)x * tile.w;
            tile.y = (int)y * tile.h;
            SDL_SetRenderDrawColor(renderer, amount2color(carn.at(x, y)),
                amount2color(plant.at(x, y)), amount2color(herb.at(x, y)), 255);
            SDL_RenderFillRect(renderer, &tile);
        }
    }
    for (unsigned y = 0; y < get_height(); ++y) {
        for (unsigned x = 0; x < get_width(); ++x) {
            Animal const& an = animal.at(x, y);
            if (an.is_present) {
                uint8_t red = an.is_carn ? 255 : 0;
                uint8_t blue = an.is_carn ? 0 : 255;
                tile.x = (an.pos.x - 0.5) * tile.w;
                tile.y = (an.pos.y - 0.5) * tile.h;
                SDL_SetRenderDrawColor(renderer, red, 0, blue, 255);
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }
}

void World::get_statistics(Statistics& stats)
{
    stats.herb_avg = Animal();
    stats.herb_count = 0;
    stats.carn_avg = Animal();
    stats.carn_count = 0;
    stats.plant_total = 0.;
    stats.herb_total = 0.;
    stats.carn_total = 0.;
    stats.baby_total = 0.;
    for (unsigned y = 0; y < get_height(); ++y) {
        for (unsigned x = 0; x < get_width(); ++x) {
            Animal const& an = animal.at(x, y);
            if (an.is_present) {
                if (an.is_carn) {
                    stats.carn_avg.add(an);
                    ++stats.carn_count;
                } else {
                    stats.herb_avg.add(an);
                    ++stats.herb_count;
                }
            }
            stats.plant_total += plant.at(x, y);
            stats.herb_total += herb.at(x, y);
            stats.carn_total += carn.at(x, y);
            stats.baby_total += baby.at(x, y);
        }
    }
    if (stats.herb_count > 0) {
        stats.herb_avg.divide((float)stats.herb_count);
    }
    if (stats.carn_count > 0) {
        stats.carn_avg.divide((float)stats.carn_count);
    }
}

void Statistics::print(FILE* to)
{
    fputs("{\"herb_avg\":", to);
    herb_avg.print(to);
    fprintf(to, ",\"herb_count\":%u", herb_count);
    fputs(",\"carn_avg\":", to);
    carn_avg.print(to);
    fprintf(to, ",\"carn_count\":%u", carn_count);
    fprintf(to, ",\"plant_total\":%f", plant_total);
    fprintf(to, ",\"herb_total\":%f", herb_total);
    fprintf(to, ",\"carn_total\":%f", carn_total);
    fprintf(to, ",\"baby_total\":%f}", baby_total);
}
