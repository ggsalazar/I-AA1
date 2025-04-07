#include "Math.h"

void Math::Clamp(float &value, const float min, const float max) {
    if (value < min) value = min;
    else if (value > max) value = max;
}

float Math::Lerp(const float a, const float b, const float t, const bool angle) {
    if (angle) {
        //Get the difference between the angles
        float diff = b - a;

        //Adjust the difference to ensure the intest path is taken
        if (diff > 180) diff -= 360;
        else if (diff < -180) diff += 360;

        return a + diff * t;
    }
    else
        return a + t * (b - a);
}

Vector2f Math::Lerp(const Vector2f a, const Vector2f b, const float t) { 
    return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t) };
}