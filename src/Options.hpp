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
    unsigned stat_interval;
    unsigned flush_interval; // 0 means no fflush calls
    unsigned frame_delay;
    int pixel_size;
    unsigned max_threads; // 0 means use the number of CPUs

    Options(int argc, char* argv[]);

    Options& operator=(Options const& copy) = default;
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_OPTIONS_H_ */
