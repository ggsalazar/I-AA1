#pragma once
#include <iostream>
#include <memory>
#include "../Core/Geometry.h"
#include "Spritesheet.h"

using namespace std;

namespace Engine {
class Sprite {
    friend class Spritesheet;
public:
    struct Info {
        Vector2u pos = { 0, 0 };
        Vector2f origin = { .0f, .0f };
        Vector2u img_size = { 1, 1 };
        Vector2u spr_size = { 0, 0 };
        Vector2f scale = { 1, 1 };
        Rect spr_rect = Rect();
        Color color = Color(1, 1, 1);
        float rot = 0.f; //Angle of rotation in degrees
        uint sheet_row = 0;
        uint num_frames = 1;
        uint curr_frame = 0;
        int fps = 0;
        uint fci = 0; //frame_change_interval: How many frames until the next frame of the animation
        float anim_length = 0.f; //Length of the animation in seconds
    };

    Sprite(unique_ptr<Spritesheet>& s, const Info& i = {}) :
        sheet(move(s)), info(i) {}
    virtual ~Sprite() {}

    inline virtual void SetSheet(unique_ptr<Spritesheet>& sht) { sheet = move(sht); }

    inline virtual void MoveTo(const Vector2u& new_pos) { info.pos = new_pos; }
    virtual void MoveBy(const Vector2i& offset);
    inline virtual void SetSize(const Vector2u& s) {
        info.spr_size = s;
        info.scale = { info.spr_size.x / info.img_size.x, info.spr_size.y / info.img_size.y };
    }
    inline virtual void SetScale(const Vector2f& s) {
        info.scale = s;
        info.spr_size = { info.img_size.x * info.scale.x, info.img_size.y * info.scale.y };
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

    inline virtual void SetFPS(const int new_fps) = 0;
    inline virtual uint GetFPS() = 0;

    virtual void SetOrigin(Vector2f new_ori = { .5f, .5f });
    inline virtual Vector2f GetOrigin() = 0; //Returns origin from 0-1
    inline virtual Vector2u GetOriginCoords() = 0; //Returns origin in pixels relative to the sprite size

protected:
    std::unique_ptr<Spritesheet> sheet;
    Info info;
};
}