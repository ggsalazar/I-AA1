#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "../../../Engine/Graphics/Sprite.h"

using namespace std;

class Sprite_SDL : public Engine::Sprite {
    friend class Renderer_SDL;
public:

    Sprite_SDL(SDL_Renderer* renderer, const Info& i = {}) :
        Sprite(i) {

        char sheet_png[100];
        strcpy(sheet_png, info.sheet.c_str());
        strcat(sheet_png, ".png");

        texture = IMG_LoadTexture(renderer, sheet_png);

        if (!texture)
            cout << "Could not load texture from file: " << info.sheet << "!\n";

        Vector2f s_size;

        SDL_GetTextureSize(texture, &s_size.x, &s_size.y);
        info.sheet_size = { (uint)s_size.x, (uint)s_size.y };
    }
    ~Sprite_SDL() { SDL_DestroyTexture(texture); }

    inline SDL_Texture* GetTexture() const { return texture; }
private:
    SDL_Texture* texture;
};