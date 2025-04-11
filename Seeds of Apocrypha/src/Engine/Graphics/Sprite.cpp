#include "Sprite.h"

void Engine::Sprite::MoveBy(const Vector2i& offset) {
    if (offset.x + info.pos.x < 0) info.pos.x = 0;
    else info.pos.x += offset.x; //How to handle offset + pos > map size?

    if (offset.y + info.pos.y < 0) info.pos.y = 0;
    else info.pos.y += offset.y; //How to handle offset + pos > map size?
}

void Engine::Sprite::SetSheetRow(uint new_s_r, const uint new_n_f) {
    //Dividing the size of the sprite rect by the height of the spritesheet should ALWAYS produce a whole number
    const uint num_rows = info.frame_size.y / sheet->GetSize().y;
    while (0 > new_s_r or new_s_r >= num_rows) {
        if (new_s_r < 0)
            new_s_r += num_rows;
        else if (new_s_r >= num_rows)
            new_s_r -= num_rows;
    }
    info.sheet_row = new_s_r;

    //If a new number of frames is not provided, it will be assumed that the # of frames is staying the same
    if (new_n_f != 0)
        SetNumFrames(new_n_f);
}

void Engine::Sprite::SetGameFPS(const uint new_fps) {
    info.game_fps = new_fps;
    info.fci = round(info.game_fps / info.anim_fps);
    info.fci = info.fci > 0 ? info.fci : info.fci * -1;
}

inline void Engine::Sprite::SetAnimFPS(const int new_fps) {
    info.anim_fps = new_fps;
    info.fci = round(info.game_fps / info.anim_fps);
    info.fci = info.fci > 0 ? info.fci : info.fci * -1;

    info.anim_length = (info.anim_fps != 0) ? info.num_frames / info.anim_fps : 0;
}

void Engine::Sprite::SetOrigin(const Vector2f new_ori) {
    if (new_ori.x < 0.f or 1.f < new_ori.x)
        info.origin.x = round(info.frame_size.x * .5);
    else info.origin.x = round(new_ori.x * info.frame_size.x);

    if (new_ori.y < 0.f or 1.f < new_ori.y)
        info.origin.y = round(info.frame_size.y * .5);
    else info.origin.y = round(new_ori.y * info.frame_size.y);
}