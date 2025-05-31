#pragma once
#include "Geometry.h"

class Collision {
public:
    inline static bool AABB(const Rect& a, const Rect& b) {
        return (a.x + a.w >= b.x && b.x + b.w >= a.x &&
                a.y + a.h >= b.y && b.y + b.h >= a.y);
    }

    inline static bool Point(const Vector2i& p, const Rect& r) {
        bool cond_x = false, cond_y = false;
        if (r.x <= p.x)
            cond_x = r.x <= p.x and p.x <= r.x + r.w;
        else
            cond_x = r.x + r.w <= p.x;

        if (r.y <= p.y)
            cond_y = r.y <= p.y and p.y <= r.y + r.h;
        else
            cond_y = r.y + r.h <= p.y;

        return cond_x and cond_y;
        //return ((r.x <= p.x or r.x+r.w <= p.x) and p.x <= r.x + r.w and
          //      (r.y <= p.y or r.y+r.h <= p.y) and p.y <= r.y + r.h);
    }
};