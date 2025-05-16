#include "Sprite.h"

Sprite::Sprite(SDL_Renderer* renderer, const Info& i)
    : info(i), pos_debug(info.pos, 4) {
    SetSize();
    SetAnimFPS(info.anim_fps);

    std::string sheet_png = "assets/Sprites/" + info.sheet + ".png";
    texture = IMG_LoadTexture(renderer, sheet_png.c_str());

    if (!texture)
        std::cout << "Could not load texture from file: " << sheet_png << "!\n";

    Vector2f s_size;
    SDL_GetTextureSize(texture, &s_size.x, &s_size.y);
    info.sheet_size = { (uint)s_size.x, (uint)s_size.y };
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
}

void Sprite::Update(const float dt) {
    //Time-based frame updates
    if (info.anim_fps != 0) {
        info.frame_timer += dt;

        if (info.frame_timer >= info.frame_length) {
            info.frame_timer -= info.frame_length;

            if (info.anim_fps > 0) SetCurrFrame(++info.curr_frame);
            else if (info.anim_fps < 0) SetCurrFrame(--info.curr_frame);
        }

        //Frame-based frame updates
        /*
        if (game_frames % fci == 0) {
            if (info.anim_fps > 0) SetCurrFrame(++info.curr_frame);
            else if (info.anim_fps < 0) SetCurrFrame(--info.curr_frame);
        }
        */
    }
}

void Sprite::MoveTo(const Vector2i& new_pos) { 
    info.pos = new_pos; 
    pos_debug.x = info.pos.x; 
    pos_debug.y = info.pos.y; 
}

void Sprite::MoveBy(const Vector2i& offset) {
    info.pos += offset;
    pos_debug.x = info.pos.x;
    pos_debug.y = info.pos.y;
}

void Sprite::SetSheetRow(uint new_s_r, const uint new_n_f) {
    //Dividing the size of the sprite rect by the height of the spritesheet should ALWAYS produce a whole number
    const uint num_rows = info.frame_size.y / info.sheet_size.y;
    while (0 > new_s_r or new_s_r >= num_rows) {
        if (new_s_r < 0) new_s_r += num_rows;
        else if (new_s_r >= num_rows) new_s_r -= num_rows;
    }
    info.sheet_row = new_s_r;

    //If a new number of frames is not provided, it will be assumed that the # of frames is staying the same
    if (new_n_f != 0)
        SetNumFrames(new_n_f);
}

void Sprite::SetCurrFrame(uint new_c_f) {
    if (info.anim_fps > 0) {
        while (new_c_f >= info.num_frames) new_c_f -= info.num_frames;
    }
    else if (info.anim_fps < 0)
        while (new_c_f < 0) new_c_f += info.num_frames;
    info.curr_frame = new_c_f;
}

void Sprite::SetGameFPS(const uint new_fps) {
    info.game_fps = new_fps;
    info.fci = round(info.game_fps / info.anim_fps);
    info.fci = info.fci > 0 ? info.fci : info.fci * -1;
}

void Sprite::SetAnimFPS(const int new_fps) {
    info.anim_fps = new_fps;

    if (info.anim_fps != 0) {
        info.fci = round(info.game_fps / info.anim_fps);
        info.fci = info.fci > 0 ? info.fci : info.fci * -1;

        info.anim_length = info.num_frames / info.anim_fps;
        info.frame_length = (info.anim_fps > 0) ? 1.f / info.anim_fps : (1.f / info.anim_fps) * -1;
    }
    else {
        info.fci = 0;
        info.anim_length = 0;
        info.frame_length = 0;
    }
}

void Sprite::SetOrigin(const Vector2f new_ori) {
    info.origin.x = (new_ori.x < 0.f or 1.f < new_ori.x) ? .5 : new_ori.x;
    info.origin.y = (new_ori.y < 0.f or 1.f < new_ori.y) ? .5 : new_ori.y;
}