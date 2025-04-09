#pragma once
#include "Geometry.h"

namespace Engine {
class Collision {
public:
    inline static bool AABB(const Rect& a, const Rect& b) {
        return (a.pos.x + a.size.x >= b.pos.x && b.pos.x + b.size.x >= a.pos.x &&
                a.pos.y + a.size.y >= b.pos.y && b.pos.y + b.size.y >= a.pos.y);
    }
};
}