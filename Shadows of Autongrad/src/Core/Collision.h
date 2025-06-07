#pragma once
#include "Geometry.h"

using std::min, std::max;

class Collision {
public:
    inline static bool AABB(const Rect& a, const Rect& b) {
        Rect a_norm = a;
        Rect b_norm = b;
        return (a.x + a.w >= b.x && b.x + b.w >= a.x &&
                a.y + a.h >= b.y && b.y + b.h >= a.y);
    }

    inline static bool LinePoint(const Line& l, const Vector2i& p) {
        //Vertical line
        if (l.x1 == l.x2) return p.x == l.x1 and min(l.y1, l.y2) <= p.y and p.y <= max(l.y1, l.y2);
        //Horizontal line
        if (l.y1 == l.y2) return p.y == l.y1 and min(l.x1, l.x2) <= p.x and p.x <= max(l.x1, l.x2);

        bool on_line = (p.y - l.y1) * (l.x2 - l.x1) == (l.y2 - l.y1) * (p.x - l.x1);
        bool within_segment = min(l.x1, l.x2) <= p.x and p.x <= max(l.x1, l.x2) and
                              min(l.y1, l.y2) <= p.y and p.y <= max(l.y1, l.y2);

        return on_line and within_segment;
    }

    inline static bool Lines(const Line& a, const Line& b) {
        return false;
    }

    inline static bool CirclePoint(const Circle& c, const Vector2i& p) {
        return false;
    }

    inline static bool CircleLine(const Circle& c, const Vector2i& p) {
        return false;
    }

    inline static bool Circles(const Circle& a, const Circle& b) {
        return false;
    }

    inline static bool RectPoint(const Rect& r, const Vector2i& p) {

        return min(r.x, r.x + r.w) <= p.x and p.x <= max(r.x, r.x + r.w) and
               min(r.y, r.y + r.h) <= p.y and p.y <= max(r.y, r.y + r.h);
    }

    inline static bool RectLine(const Rect& r, const Line& l) {

        return false;
    }

    inline static bool RectCircle(const Rect& r, const Circle& c) {
        return false;
    }
};