#pragma once
#include "Geometry.h"

class Collision {
public:
    inline static bool AABB(const Rect& a, const Rect& b) {
        return (a.x + a.w >= b.x && b.x + b.w >= a.x &&
                a.y + a.h >= b.y && b.y + b.h >= a.y);
    }

    inline static bool Point(const Vector2f& p, const Rect& r) {
        return (r.x <= p.x and p.x <= r.x + r.w and
                r.y <= p.y and p.y <= r.y + r.h);
    }
};