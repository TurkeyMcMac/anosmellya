#ifndef ANOSMELLYA_CONFIG_H_
#define ANOSMELLYA_CONFIG_H_

namespace anosmellya {

struct Config {
public:
    // Initializes the default values. See the file "conf" for said defaults.
    Config();

    int parse(const char* path);

    int parse_cstr(const char* cstr);

    float acceleration;
    float baby_dispersal;
    float baby_evap;
    float carn_amount;
    float carn_dispersal;
    float carn_eat_portion;
    float carn_evap;
    float friction;
    float herb_amount;
    float herb_dispersal;
    float herb_eat_portion;
    float herb_evap;
    float initial_animal_chance;
    float initial_carn_chance;
    float initial_variation;
    float lifespan;
    float mutate_amount;
    float mutate_chance;
    float plant_dispersal;
    float plant_evap;
    float plant_place_amount;
    float plant_place_chance;
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_CONFIG_H_ */
