#pragma once
#include <d2d1.h>
#include "../../Engine/Graphics/Sprite.h"
#include "Spritesheet_D2D.h"

namespace Engine {

class Sprite_D2D : public Sprite {
    friend class Spritesheet_D2D;
public:

    Sprite_D2D(const Info& i, const Transform& t = {}) :
        Sprite(i, t) {}

    void SetSheet(const std::unique_ptr<Spritesheet> sht) {};

    void SetPos(const Vector2u& p) {};
    void SetSize(const Vector2u& s) {};
    void SetScale(const Vector2u& s) {};
    void SetRot(float angle) {};
    void SetColor(const Color& color) {};

    void SetSheetRow(uint new_s_r, const int new_n_f = -1) {};
    void GetSheetRow() {};

    void SetCurrFrame(int new_c_f) {};
    uint GetCurrFrame() {};

    void SetNumFrames(const int new_n_f) {};
    uint GetNumFrames() {};

    void SetFPS(const int new_fps) {};
    uint GetFPS() {};

    void SetOrigin(Vector2f new_ori = { .5f, .5f }) {};
    Vector2f GetOrigin() {}; //Returns origin from 0-1
    Vector2u GetOriginCoords() {}; //Returns origin in pixels relative to the sprite size

};

}