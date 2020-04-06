#ifndef ANOSMELLYA_ANIMAL_H_
#define ANOSMELLYA_ANIMAL_H_

#include "Random.hpp"
#include "Vec2D.hpp"

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

    //Animal(Random& random, float carnivore_chance);

    Animal(Random& random, Animal& mother, Animal& father);

    void be_carn();

    void be_herb();

    void mutate(Random& random, float mutate_amount);

    void position(Vec2D position);

    /* INDIVIDUAL ATTRIBUTES */
    Vec2D pos;
    Vec2D vel;
    float food;
    unsigned age;
    bool is_present;
    bool just_moved;
    /* GENETICS */
    bool is_carn;
    float mutate_chance;
    float mutate_amount;
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
