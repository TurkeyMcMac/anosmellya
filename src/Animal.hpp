#ifndef ANOSMELLYA_ANIMAL_H_
#define ANOSMELLYA_ANIMAL_H_

#include "Random.hpp"
#include "Vec2D.hpp"
#include <stdio.h>

namespace anosmellya {

struct SmellAffinity {
    Vec2D impulse;
    float plant_effect;
    float herb_effect;
    float carn_effect;
    float baby_effect;
    float food_effect;

    SmellAffinity();

    SmellAffinity(SmellAffinity const& copy);
};

struct Animal {
    Animal();

    Animal(Random& random, Animal& mother, Animal& father);

    void be_carn();

    void be_herb();

    void mutate(Random& random, float amount);

    void add(Animal const& an);

    void divide(float d);

    void print(FILE* to);

    static constexpr float MUTATE_CHANCE = 0.05;

    static constexpr float MUTATE_AMOUNT = 4.;

    /* INDIVIDUAL ATTRIBUTES */
    Vec2D pos;
    Vec2D vel;
    float food;
    unsigned age;
    bool is_present;
    bool just_moved;
    /* GENETICS */
    bool is_carn;
    float baby_smell_amount;
    float baby_threshold;
    float baby_food;
    SmellAffinity plant_aff;
    SmellAffinity herb_aff;
    SmellAffinity carn_aff;
    SmellAffinity baby_aff;
    SmellAffinity vel_aff;
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_ANIMAL_H_ */
