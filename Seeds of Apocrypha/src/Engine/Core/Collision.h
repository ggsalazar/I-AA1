#pragma once
#include "Geometry.h"

class Collision {
public:
    static bool AABB(const Rect& a, const Rect& b) {
        return (a.pos.x + a.size.x >= b.pos.x and b.pos.x + b.size.x >= a.pos.x and
                a.pos.y + a.size.y >= b.pos.y and b.pos.y + b.size.y >= a.pos.y);
    }
};