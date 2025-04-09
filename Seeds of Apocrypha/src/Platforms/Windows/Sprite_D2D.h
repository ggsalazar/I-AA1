#pragma once
#include <d2d1.h>
#include "../../Engine/Graphics/Sprite.h"

namespace Engine {

class Sprite_D2D : public Sprite {
public:
	Sprite_D2D(std::unique_ptr<Spritesheet> sprsht, Vector2u s = {1, 1}) : spritesheet(std::move(sprsht)), scale(s) {}




    
    Vector2u pos = { 0, 0 };
    Vector2u scale; //Int BECAUSE this is a pixel art game!
    float rotation = 0;
    Color color = Color(1, 1, 1);

private:
    std::unique_ptr<Spritesheet> spritesheet;
};

}