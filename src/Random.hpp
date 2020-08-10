#ifndef ANOSMELLYA_RANDOM_H_
#define ANOSMELLYA_RANDOM_H_

#include <stdint.h>

class Random {
public:
    static const uint32_t MAX_INT = 0xFFFFFFFF;

    Random(uint32_t seed)
        : state(seed)
    {
    }

    Random& operator=(Random const& copy) = default;

    uint32_t generate()
    {
        // https://en.wikipedia.org/wiki/Linear_congruential_generator (maybe?)
        state = 1103515245 * state + 12345;
        return state;
    }

    float generate(float max)
    {
        return (float)generate() / (MAX_INT + 1.) * max;
    }

    float generate_pos_neg(float max) { return generate(max * 2.) - max; }

private:
    uint32_t state;
};

#endif /* ANOSMELLYA_RANDOM_H_ */
