#include "Animal.hpp"
#include <cmath>

using namespace anosmellya;

SmellAffinity::SmellAffinity()
    : impulse(0., 0.)
    , plant_effect(0.)
    , herb_effect(0.)
    , carn_effect(0.)
    , baby_effect(0.)
    , food_effect(0.)
{
}

SmellAffinity::SmellAffinity(SmellAffinity const& copy)
    : impulse(copy.impulse)
    , plant_effect(copy.plant_effect)
    , herb_effect(copy.herb_effect)
    , carn_effect(copy.carn_effect)
    , baby_effect(copy.baby_effect)
    , food_effect(copy.food_effect)
{
}

Animal::Animal()
    : pos(0., 0.)
    , vel(0., 0.)
    , food(0.)
    , age(0)
    , is_present(false)
    , just_moved(false)
    , is_carn(false)
    , mutate_chance(0.)
    , mutate_amount(0.)
    , baby_smell_amount(0.)
    , baby_threshold(0.)
    , baby_food(0.)
    , plant_aff()
    , herb_aff()
    , carn_aff()
    , baby_aff()
    , vel_aff()
{
}

static float pick(Random& random, float a, float b)
{
    return random.generate() % 2 == 0 ? a : b;
}

static SmellAffinity pick_aff(
    Random& random, SmellAffinity const& a, SmellAffinity const& b)
{
    SmellAffinity aff;
    aff.impulse.x = pick(random, a.impulse.x, b.impulse.x);
    aff.impulse.y = pick(random, a.impulse.y, b.impulse.y);
    aff.plant_effect = pick(random, a.plant_effect, b.plant_effect);
    aff.herb_effect = pick(random, a.herb_effect, b.herb_effect);
    aff.carn_effect = pick(random, a.carn_effect, b.carn_effect);
    aff.baby_effect = pick(random, a.baby_effect, b.baby_effect);
    aff.food_effect = pick(random, a.food_effect, b.food_effect);
    return aff;
}

Animal::Animal(Random& random, Animal& mother, Animal& father)
    : pos(mother.pos)
    , vel(mother.vel)
    , food(mother.baby_food)
    , age(0)
    , is_present(true)
    , is_carn(mother.is_carn)
    , mutate_chance(pick(random, mother.mutate_chance, father.mutate_chance))
    , mutate_amount(pick(random, mother.mutate_amount, father.mutate_amount))
    , baby_smell_amount(
          pick(random, mother.baby_smell_amount, father.baby_smell_amount))
    , baby_threshold(pick(random, mother.baby_threshold, father.baby_threshold))
    , baby_food(pick(random, mother.baby_threshold, father.baby_threshold))
    , plant_aff(pick_aff(random, mother.plant_aff, father.plant_aff))
    , herb_aff(pick_aff(random, mother.herb_aff, father.herb_aff))
    , carn_aff(pick_aff(random, mother.carn_aff, father.carn_aff))
    , baby_aff(pick_aff(random, mother.baby_aff, father.baby_aff))
    , vel_aff(pick_aff(random, mother.vel_aff, father.vel_aff))
{
    mother.food -= food;
    if (mother.food < 0) {
        food += mother.food;
        mother.food = 0;
    }
    if (mother.mutate_chance > random.generate(1.)) {
        mutate(random, mother.mutate_chance);
    }
}

void Animal::be_carn()
{
    food = 100.;
    age = 0.;
    is_present = true;
    is_carn = true;
    mutate_chance = 0.02;
    mutate_amount = 0.1;
    baby_smell_amount = 1.;
    baby_threshold = 100.;
    baby_food = 50.;
    plant_aff = SmellAffinity();
    herb_aff = SmellAffinity();
    herb_aff.impulse.x = 1.;
    carn_aff = SmellAffinity();
    baby_aff = SmellAffinity();
    baby_aff.impulse.x = 1.;
    vel_aff = SmellAffinity();
    vel_aff.impulse.x = -0.2;
}

void Animal::be_herb()
{
    food = 100.;
    age = 0.;
    is_present = true;
    is_carn = false;
    mutate_chance = 0.02;
    mutate_amount = 0.1;
    baby_smell_amount = 1.;
    baby_threshold = 100.;
    baby_food = 50.;
    plant_aff = SmellAffinity();
    plant_aff.impulse.x = 1.;
    herb_aff = SmellAffinity();
    carn_aff = SmellAffinity();
    carn_aff.impulse.x = -1.;
    baby_aff = SmellAffinity();
    baby_aff.impulse.x = 1.;
    vel_aff = SmellAffinity();
    vel_aff.impulse.x = -0.2;
}

static void mutate_affinity(
    SmellAffinity& aff, Random& random, float mutate_amount)
{
    aff.impulse.x += random.generate_pos_neg(mutate_amount);
    aff.impulse.y += random.generate_pos_neg(mutate_amount);
    aff.plant_effect += random.generate_pos_neg(mutate_amount);
    aff.herb_effect += random.generate_pos_neg(mutate_amount);
    aff.carn_effect += random.generate_pos_neg(mutate_amount);
    aff.food_effect += random.generate_pos_neg(mutate_amount);
}

void Animal::mutate(Random& random, float mutate_amount)
{
    mutate_chance += random.generate_pos_neg(mutate_amount);
    mutate_amount += random.generate_pos_neg(mutate_amount);
    baby_smell_amount += random.generate_pos_neg(mutate_amount);
    baby_threshold += random.generate_pos_neg(mutate_amount);
    baby_food += random.generate_pos_neg(mutate_amount);
    mutate_affinity(plant_aff, random, mutate_amount);
    mutate_affinity(herb_aff, random, mutate_amount);
    mutate_affinity(carn_aff, random, mutate_amount);
    mutate_affinity(baby_aff, random, mutate_amount);
    mutate_affinity(vel_aff, random, mutate_amount);
}

void Animal::position(Vec2D position) { pos = position; }
