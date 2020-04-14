#ifndef ANOSMELLYA_VEC2D_H_
#define ANOSMELLYA_VEC2D_H_

namespace anosmellya {

struct Vec2D {
    float x;
    float y;

    Vec2D(float xx, float yy)
        : x(xx)
        , y(yy)
    {
    }

    Vec2D()
        : Vec2D(0., 0.)
    {
    }

    Vec2D& operator=(Vec2D const& copy) = default;
};

} /* namespace anosmellya */

#endif /* ANOSMELLYA_VEC2D_H_ */
