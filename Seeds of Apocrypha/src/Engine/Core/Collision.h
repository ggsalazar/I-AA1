#pragma once
#include "Geometry.h"

namespace Engine {
class Collision {
public:
    inline static bool AABB(const Rect& a, const Rect& b) {
        return (a.pos.x + a.size.x >= b.pos.x && b.pos.x + b.size.x >= a.pos.x &&
                a.pos.y + a.size.y >= b.pos.y && b.pos.y + b.size.y >= a.pos.y);
    }

    inline static bool Point(const Vector2f& p, const Rect& r) {
        return (r.pos.x <= p.x and p.x <= r.pos.x + r.size.x and
                r.pos.y <= p.y and p.y <= r.pos.y + r.size.y);
    }
};
}