#pragma once
#include <memory>
#include "../Core/Geometry.h"
#include "Spritesheet.h"

namespace Engine {
class Sprite {
public:
    virtual void SetPosition(const Vector2u& p) = 0;
    virtual void SetScale(const Vector2u& s) = 0;
    virtual void SetRotation(float angle) = 0;
    virtual void SetColor(const Color& color) = 0;
};
}