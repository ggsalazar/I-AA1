#pragma once
#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "../Core/Aliases.h"
#include "../Core/Geometry.h"

class Sprite {
    friend class Renderer;
public:
    struct Info {
        std::string sheet = "";
        Vector2i pos; //Worldspace position of the sprite
        Vector2f origin; //Anchor point of the sprite
        Vector2u spr_size; //The size of the sprite in world space (i.e. after being scaled up/down)
        Vector2u sheet_size; //The size of the sprite's spritesheet
        Vector2i scale{1}; //The scale of the sprite - int BECAUSE this is a pixel art game!
        Vector2u frame_size{1}; //The literal, actual size of a single frame of the sprite
        Color tint{1}; //The color tint of the sprite
        float rot = 0.f; //Angle of rotation in degrees
        uint sheet_row = 0; //Which row of the sheet our current animation is on (each row should be a different animation)
        uint num_frames = 1; //How many frames are in the CURRENT sheet row
        uint curr_frame = 0; //Which frame of the animation we are currently on
        int anim_fps = 0; //How many frames of the animation play per second
        uint game_fps = 60; //The fps of the game
        uint fci = 0; //frame_change_interval: How many game frames until the next animation frame
        float anim_length = 0.f; //Length of the animation in seconds
        float frame_length = 0.f; //Length of a single frame in seconds
        float frame_timer = 0.f; //How long since the frame changed
        int dfc = 0; //Distance from camera; draw order, basically - the lower the number, the closer to the camera
    };
    Circle pos_debug;

    Sprite(SDL_Renderer* renderer, const Info& i = {});
    ~Sprite() { SDL_DestroyTexture(texture); }

    inline SDL_Texture* GetTexture() const { return texture; }

    void Update(const float dt);


    //Spritesheet stuff
    inline Vector2u GetSheetSize() const { return info.sheet_size; }

    //Sprite proper stuff
    inline Vector2i GetPos() const { return info.pos; }
    inline void MoveTo(const Vector2i& new_pos) { info.pos = new_pos; }
    inline void MoveBy(const Vector2i& offset) { info.pos += offset; }

    inline void SetSize() { info.spr_size = { info.scale.x * info.frame_size.x, info.scale.y * info.frame_size.y }; }
    inline void SetSize(const Vector2u& s) {
        info.spr_size = s;
        info.scale = { (int)(info.spr_size.x / info.frame_size.x), (int)(info.spr_size.y / info.frame_size.y) };
    }
    inline Vector2u GetSprSize() const { return info.spr_size; }
    inline Vector2u GetFrameSize() const { return info.frame_size; }

    inline void SetScale(const Vector2i& s) {
        info.scale = s;
        info.spr_size = { info.frame_size.x * info.scale.x, info.frame_size.y * info.scale.y };
    }
    inline Vector2i GetScale() const { return info.scale; }

    //Rotation in degrees
    inline void SetRotD(float angle) { info.rot = angle; }
    inline float GetRotD() const { return info.rot; }
    //Rotation in radians
    inline void SetRotR(float rad) { info.rot = rad * 57.2958; }
    inline float GetRotR() const { return info.rot / 57.2958; }
    
    inline void SetColor(const Color& c) { info.tint = c; }
    inline Color GetColor() const { return info.tint; }

    void SetSheetRow(uint new_s_r, const uint new_n_f = 0);
    inline uint GetSheetRow() const { return info.sheet_row; }

    void SetCurrFrame(uint new_c_f);
    inline uint GetCurrFrame() const { return info.curr_frame; }

    inline void SetNumFrames(const uint new_n_f) { info.num_frames = new_n_f; }
    inline uint GetNumFrames() const { return info.num_frames; }

    void SetGameFPS(const uint new_fps);
    inline uint GetGameFPS() const { return info.game_fps; }
    void SetAnimFPS(const int new_fps);
    inline uint GetAnimFPS() const { return info.anim_fps; }

    void SetOrigin(const Vector2f new_ori = { .5f, .5f });
    inline Vector2f GetOrigin() const { return info.origin; }; //Returns origin from 0-1
    inline Vector2f GetOriginFrame() { return { info.origin.x * info.frame_size.x, info.origin.y * info.frame_size.y }; }; //Returns origin in pixels relative to the frame size
    inline Vector2f GetOriginSprite() { return { info.origin.x * info.spr_size.x, info.origin.y * info.spr_size.y }; }; //Returns origin in pixels relative to the sprite size

    inline void SetDFC(const int new_dfc = 0) { info.dfc = new_dfc; }
    inline int GetDFC() const { return info.dfc; }

private:
    Info info; //private because whenever a member is set, other ancillary functions must be performed
    SDL_Texture* texture;
};