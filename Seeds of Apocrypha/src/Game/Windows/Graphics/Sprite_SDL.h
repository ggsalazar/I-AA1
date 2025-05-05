#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "../../../Engine/Graphics/Sprite.h"

class Sprite_SDL : public Engine::Sprite {
    friend class Renderer_SDL;
public:

    Sprite_SDL(SDL_Renderer* renderer, const Info& i = {}) :
        Sprite(i) {

        std::string sheet_png = info.sheet + ".png";
        texture = IMG_LoadTexture(renderer, sheet_png.c_str());

        if (!texture)
            std::cout << "Could not load texture from file: " << info.sheet << "!\n";

        Vector2f s_size;
        SDL_GetTextureSize(texture, &s_size.x, &s_size.y);
        info.sheet_size = { (uint)s_size.x, (uint)s_size.y };
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    }
    ~Sprite_SDL() { SDL_DestroyTexture(texture); }

    inline SDL_Texture* GetTexture() const { return texture; }
private:
    SDL_Texture* texture;
};