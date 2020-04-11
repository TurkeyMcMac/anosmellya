#ifndef ANOSMELLYA_OPTIONS_H_
#define ANOSMELLYA_OPTIONS_H_

#include "Config.hpp"
#include <stdint.h>

namespace anosmellya {

struct Options {
    Config conf;
    int world_width;
    int world_height;
    uint32_t seed;
    bool draw;
    bool print_stats;
    unsigned frame_delay;
    int pixel_size;

    Options(int argc, char* argv[]);
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_OPTIONS_H_ */
