#ifndef ANOSMELLYA_GRID_H_
#define ANOSMELLYA_GRID_H_

#include <stdio.h>
#include <stdlib.h>

namespace anosmellya {

template <typename T> class Grid {
public:
    Grid()
        : width(0)
        , height(0)
        , tiles(NULL)
    {
    }

    Grid& operator=(Grid const& copy) = default;

    Grid(unsigned width, unsigned height)
        : width(width)
        , height(height)
        , tiles((T*)calloc(width * sizeof(T), height))
    {
        check_oom();
    }

    Grid(unsigned width, unsigned height, T fill)
        : width(width)
        , height(height)
        , tiles((T*)malloc(width * height * sizeof(T)))
    {
        check_oom();
        for (unsigned i = 0; i < width * height; ++i) {
            tiles[i] = fill;
        }
    }

    ~Grid() { free(tiles); }

    T& at(unsigned x, unsigned y) { return tiles[y * width + x]; }

    // Offset x and y by ox and oy units, respectively, wrapping if needed.
    void trans(unsigned& x, unsigned& y, int ox, int oy)
    {
        ox %= width;
        oy %= height;
        small_trans(x, y, ox, oy);
    }

    // Translate and fetch.
    T& at_trans(unsigned x, unsigned y, int ox, int oy)
    {
        trans(x, y, ox, oy);
        return at(x, y);
    }

    // Same as trans, but ox and oy must have absolute values less than width
    // and height, respectively.
    void small_trans(unsigned& x, unsigned& y, int ox, int oy)
    {
        if (ox < 0) {
            unsigned uox = -ox;
            if (x < uox) {
                x = width - (uox - x);
            } else {
                x -= uox;
            }
        } else {
            unsigned uox = ox;
            x += uox;
            if (x >= width) {
                x -= width;
            }
        }
        if (oy < 0) {
            unsigned uoy = -oy;
            if (y < uoy) {
                y = height - (uoy - y);
            } else {
                y -= uoy;
            }
        } else {
            unsigned uoy = oy;
            y += uoy;
            if (y >= height) {
                y -= height;
            }
        }
    }

    // Small translate and fetch.
    T& at_small_trans(unsigned x, unsigned y, int ox, int oy)
    {
        small_trans(x, y, ox, oy);
        return at(x, y);
    }

    void fill(T with)
    {
        for (unsigned i = 0; i < width * height; ++i) {
            tiles[i] = with;
        }
    }

    unsigned get_width() { return width; }

    unsigned get_height() { return height; }

private:
    unsigned width;
    unsigned height;
    T* tiles;

    void check_oom()
    {
        if (!tiles) {
            fputs("Out of memory.\n", stderr);
            abort();
        }
    }
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_GRID_H_ */
