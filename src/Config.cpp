#include "Config.hpp"
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <stddef.h>
#include <stdlib.h>
#include <string>

using namespace anosmellya;

Config::Config()
    : acceleration(0.05)
    , baby_dispersal(0.2)
    , baby_evap(0.001)
    , carn_amount(1.)
    , carn_dispersal(0.4)
    , carn_eat_portion(0.5)
    , carn_evap(0.0005)
    , friction(0.03)
    , herb_amount(1.)
    , herb_dispersal(0.4)
    , herb_eat_portion(0.5)
    , herb_evap(0.0005)
    , initial_animal_chance(0.1)
    , initial_carn_chance(0.1)
    , initial_variation(100.)
    , lifespan(500)
    , mutate_amount(4.)
    , mutate_chance(0.05)
    , plant_dispersal(0.07)
    , plant_evap(0.001)
    , plant_place_amount(800.)
    , plant_place_chance(0.000063492)
{
}

static size_t skip_space(std::string& line, size_t i)
{
    while (i < line.length() && isspace(line[i])) {
        ++i;
    }
    if (i < line.length() && line[i] == '#') {
        return line.length();
    } else {
        return i;
    }
}

static bool is_key_char(int ch) { return isalnum(ch) || ch == '_'; }

static bool set_option(Config& conf, std::string& key, float val)
{
    if (key == "acceleration") {
        conf.acceleration = val;
    } else if (key == "baby_dispersal") {
        conf.baby_dispersal = val;
    } else if (key == "baby_evap") {
        conf.baby_evap = val;
    } else if (key == "carn_amount") {
        conf.carn_amount = val;
    } else if (key == "carn_dispersal") {
        conf.carn_dispersal = val;
    } else if (key == "carn_eat_portion") {
        conf.carn_eat_portion = val;
    } else if (key == "carn_evap") {
        conf.carn_evap = val;
    } else if (key == "friction") {
        conf.friction = val;
    } else if (key == "herb_amount") {
        conf.herb_amount = val;
    } else if (key == "herb_dispersal") {
        conf.herb_dispersal = val;
    } else if (key == "herb_eat_portion") {
        conf.herb_eat_portion = val;
    } else if (key == "herb_evap") {
        conf.herb_evap = val;
    } else if (key == "initial_variation") {
        conf.initial_variation = val;
    } else if (key == "initial_animal_chance") {
        conf.initial_animal_chance = val;
    } else if (key == "initial_carn_chance") {
        conf.initial_carn_chance = val;
    } else if (key == "lifespan") {
        conf.lifespan = val;
    } else if (key == "mutate_amount") {
        conf.mutate_amount = val;
    } else if (key == "mutate_chance") {
        conf.mutate_chance = val;
    } else if (key == "plant_dispersal") {
        conf.plant_dispersal = val;
    } else if (key == "plant_evap") {
        conf.plant_evap = val;
    } else if (key == "plant_place_amount") {
        conf.plant_place_amount = val;
    } else if (key == "plant_place_chance") {
        conf.plant_place_chance = val;
    } else {
        return false;
    }
    return true;
}

static int parse_istream(Config& conf, std::istream& file, const char* name)
{
    std::string line;
    for (std::getline(file, line); file.good(); std::getline(file, line)) {
        size_t i = skip_space(line, 0);
        if (i >= line.length()) {
            continue;
        }
        size_t key_start = i;
        size_t key_end;
        for (key_end = i; key_end < line.length() && is_key_char(line[key_end]);
             ++key_end) { }
        i = skip_space(line, key_end);
        if (i >= line.length() || line[i] != '=') {
            fprintf(
                stderr, "No configuration value on line: %s\n", line.c_str());
            return -1;
        }
        i = skip_space(line, i + 1);
        char* val_end;
        const char* val_start = line.c_str() + i;
        float val = strtod(val_start, &val_end);
        if (val_start == val_end) {
            fprintf(
                stderr, "No configuration value on line: %s\n", line.c_str());
            return -1;
        }
        if (*val_end != '\0' && *val_end != '#' && !isspace(*val_end)) {
            fprintf(stderr, "Invalid configuration value: %s\n", val_start);
            return -1;
        }
        std::string key = line.substr(key_start, key_end - key_start);
        if (!set_option(conf, key, val)) {
            fprintf(stderr, "Unknown configuration option: %s\n", key.c_str());
            return -1;
        }
    }
    if (file.fail() && !file.eof()) {
        fprintf(stderr, "Failed while reading configuration file '%s'\n", name);
        return -1;
    }
    return 0;
}

int Config::parse(const char* path)
{
    std::ifstream file;
    file.open(path, std::ifstream::in);
    if (file.fail()) {
        fprintf(stderr, "Failed to open configuration file '%s'\n", path);
        return -1;
    }
    return parse_istream(*this, file, path);
}

int Config::parse_cstr(const char* cstr)
{
    std::string str(cstr);
    str.append("\n");
    std::stringstream stream(str);
    return parse_istream(*this, stream, "(string)");
}
