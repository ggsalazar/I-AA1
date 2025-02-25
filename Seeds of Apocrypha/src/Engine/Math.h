#pragma once
#include <iostream>
#include "Utils/Macros.h"

namespace Math {
    //Math/Numbers
    void Clamp(float &value, const float min, const float max);
    float Lerp(const float a, const float b, const float t, const bool angle = false);
    //sf::Vector2f Lerp(const sf::Vector2f a, const sf::Vector2f b, const float t);
};