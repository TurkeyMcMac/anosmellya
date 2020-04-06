#ifndef ANOSMELLYA_GRID_H_
#define ANOSMELLYA_GRID_H_

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

    Grid(unsigned width, unsigned height)
        : width(width)
        , height(height)
        , tiles((T*)calloc(width * sizeof(T), height))
    {
    }

    Grid(unsigned width, unsigned height, T fill)
        : width(width)
        , height(height)
        , tiles((T*)malloc(width * height * sizeof(T)))
    {
        for (unsigned i = 0; i < width * height; ++i) {
            tiles[i] = fill;
        }
    }

    ~Grid() { free(tiles); }

    T& at(unsigned x, unsigned y) { return tiles[y * width + x]; }

    void trans(unsigned& x, unsigned& y, int ox, int oy)
    {
        ox %= width;
        oy %= height;
        small_trans(x, y, ox, oy);
    }

    T& at_trans(unsigned x, unsigned y, int ox, int oy)
    {
        trans(x, y, ox, oy);
        return at(x, y);
    }

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
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_GRID_H_ */
