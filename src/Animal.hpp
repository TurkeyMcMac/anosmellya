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

    // Construct a smell affinity with all zeroes.
    SmellAffinity();

    SmellAffinity& operator=(SmellAffinity const& copy) = default;
};

struct Animal {
    // Construct an animal with all zeroes.
    Animal();

    Animal& operator=(Animal const& copy) = default;

    // Construct an animal with a random mix of traits from the mother and
    // father. The new animal has the position and velocity of the mother. Its
    // starting food is the minimum of the mother's food and the mother's baby
    // food, or zero if the quantity would otherwise be negative. No food is
    // taken from the mother. The child is never a mutant.
    Animal(Random& random, Animal const& mother, Animal const& father);

    // Initialize an animal to be a decent starting carnivore. The position must
    // be set after this.
    void be_carn();

    // Initialize an animal to be a decent starting herbivore. The position must
    // be set after this.
    void be_herb();

    // Mutate all traits by a quantity between -amount and +amount.
    void mutate(Random& random, float amount);

    // Add all statistically relevant traits of this animal and the other.
    void add(Animal const& an);

    // Divide all statistically relevant traits of this animal by the positive
    // divisor d.
    void divide(float d);

    // Print all statistically relevant traits to the file.
    void print(FILE* to);

    /* INDIVIDUAL ATTRIBUTES */
    Vec2D pos;
    Vec2D vel;
    float food;
    unsigned age;
    // Whether there is a living animal in this animal's place.
    bool is_present;
    // Whether the animal was moved this tick already.
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
