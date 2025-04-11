#pragma once
#include <iostream>
#include <memory>
#include "../Core/Geometry.h"
#include "Spritesheet.h"

using namespace std;

class Game;

namespace Engine {
class Sprite {
    friend class Spritesheet;
    friend class Renderer;
public:
    struct Info {
        Vector2u pos; //Worldspace position of the sprite
        Vector2f origin; //Anchor point of the sprite
        Vector2u spr_size; //The size of the sprite in world space (i.e. after being scaled up/down)
        Vector2u scale{1}; //The scale of the sprite - uint BECAUSE this is a pixel art game!
        Vector2u frame_size{1}; //The literal, actual size of a single frame of the sprite
        Color color{ 1, 1, 1 }; //The color tint of the sprite
        float rot{}; //Angle of rotation in degrees
        uint sheet_row{}; //Which row of the sheet our current animation is on (each row should be a different animation)
        uint num_frames{1}; //How many frames are in the CURRENT sheet row
        uint curr_frame{}; //Which frame of the animation we are currently on
        int anim_fps{}; //How many frames of the animation play per second
        uint game_fps{60}; //The fps of the game
        uint fci{}; //frame_change_interval: How many game frames until the next animation frame
        float anim_length{}; //Length of the animation in seconds
        float frame_length{}; //Length of a single frame in seconds
        float frame_timer{}; //How long since the frame changed
    };

    Sprite(unique_ptr<Spritesheet>& s, const Info& i = {}) :
        sheet(move(s)), info(i) {
        SetSize();
        SetAnimFPS(info.anim_fps);

        cout << "Frame length: " << info.frame_length << endl;
    }
    virtual ~Sprite() {}

    inline virtual void SetSheet(unique_ptr<Spritesheet>& sht) { sheet = move(sht); }

    virtual void Update(const Game& game) = 0;


    inline Vector2u GetPos() const { return info.pos; }
    inline virtual void MoveTo(const Vector2u& new_pos) { info.pos = new_pos; }
    virtual void MoveBy(const Vector2i& offset);

    inline virtual void SetSize() { info.spr_size = info.scale * info.frame_size; }
    inline virtual void SetSize(const Vector2u& s) {
        info.spr_size = s;
        info.scale = info.spr_size / info.frame_size;
    }
    inline virtual void SetScale(const Vector2u& s) {
        info.scale = s;
        info.spr_size = info.frame_size * info.scale;
    }

    inline virtual void SetRotD(float angle) { info.rot = angle; }
    inline virtual float GetRotD() const { return info.rot; }
    inline virtual void SetRotR(float rad) { info.rot = rad * 57.2958; }
    inline virtual float GetRotR() const { return info.rot / 57.2958; }
    
    inline virtual void SetColor(const Color& c) { info.color = c; }

    virtual void SetSheetRow(uint new_s_r, const uint new_n_f = 0);
    inline virtual uint GetSheetRow() const { return info.sheet_row; }

    virtual void SetCurrFrame(uint new_c_f) { while (new_c_f >= info.num_frames) new_c_f -= info.num_frames; info.curr_frame = new_c_f; }
    inline virtual uint GetCurrFrame() const { return info.curr_frame; }

    inline virtual void SetNumFrames(const uint new_n_f) { info.num_frames = new_n_f; }
    inline virtual uint GetNumFrames() const { return info.num_frames; }

    virtual void SetGameFPS(const uint new_fps);
    inline virtual uint GetGameFPS() const { return info.game_fps; }
    virtual void SetAnimFPS(const int new_fps);
    inline virtual uint GetAnimFPS() const { return info.anim_fps; }

    virtual void SetOrigin(const Vector2f new_ori = { .5f, .5f });
    inline virtual Vector2f GetOrigin() const { return info.origin; }; //Returns origin from 0-1
    inline virtual Vector2f GetOriginImgCoords() { return { info.origin.x * info.frame_size.x, info.origin.y * info.frame_size.y }; }; //Returns origin in pixels relative to the image size
    inline virtual Vector2f GetOriginSprCoords() { return { info.origin.x * info.spr_size.x, info.origin.y * info.spr_size.y }; }; //Returns origin in pixels relative to the sprite size

protected:
    std::unique_ptr<Spritesheet> sheet;
    Info info;
};
}