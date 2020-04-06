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

    uint32_t generate()
    {
        static const uint32_t rand_table[16]
            = { 17, 11, 4, 15, 14, 2, 7, 1, 13, 11, 13, 14, 12, 15, 19, 5 };
        uint32_t r = rotright(state, (state | 1) & 31) + rand_table[state % 16];
        state = r;
        return r;
    }

    float generate(float max)
    {
        return (float)generate() / (MAX_INT + 1.) * max;
    }

    float generate_pos_neg(float max)
    {
    	return generate(max * 2.) - max;
    }

private:
    uint32_t state;

    static uint32_t rotright(uint32_t bits, uint32_t amount)
    {
        return bits >> amount | bits << (32 - amount);
    }
};

#endif /* ANOSMELLYA_RANDOM_H_ */
