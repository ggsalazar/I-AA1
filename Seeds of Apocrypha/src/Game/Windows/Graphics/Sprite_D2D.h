#pragma once
#include <d2d1.h>
#include "../../../Engine/Graphics/Sprite.h"
#include "Spritesheet_D2D.h"

class Game;

namespace Engine {

class Sprite_D2D : public Sprite {
    friend class Spritesheet_D2D;
    friend class Renderer_D2D;
public:

    //Don't forget to add Entity* when finished implementing!
    //Sprite_D2D(unique_ptr<Spritesheet>& s, Entity* e, const Info& i = {}) :
    Sprite_D2D(unique_ptr<Spritesheet> s, const Info& i = {}) :
        Sprite(s, i) {}

    void Update(const Game& game) override;

};

}