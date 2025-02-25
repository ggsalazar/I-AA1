#include "Animation.h"
#include "../Entities/Entity.h"

Animation::Animation(Game& g, sf::RenderWindow& w, Entity* e, const AnimInfo& a_i, const Transform& t) :
    game(g), window(w), anim_info(a_i), 
    texture("assets/Sprites/" + anim_info.tex_name + ".png"), sprite(texture) {

    texture.setSmooth(false);

    SetSpriteW(anim_info.sprite_w);
    SetSpriteH(anim_info.sprite_h);
    SetTexRow(anim_info.tex_row, anim_info.num_frames); //STR will set the # of frames
    SetCurrFrame(anim_info.curr_frame);
    SetFPS(anim_info.fps);

    //Set the origin/anchor, scale, and position
    SetOrigin(t.origin);
    sprite.setScale(t.scale);
    sprite.setPosition(t.pos);

    //Entity MUST be initialized at the end due it being referenced in SetSpriteW/H
    entity = e;
}

void Animation::Draw() {
    //Set the current frame in the spritesheet
    tex_rect = sf::IntRect(sf::Vector2i(anim_info.curr_frame * anim_info.sprite_w, anim_info.tex_row * anim_info.sprite_h), sf::Vector2i(anim_info.sprite_w, anim_info.sprite_h));
    sprite.setTextureRect(tex_rect);

    //Go to the next frame in the spritesheet
    if (anim_info.fci != 0 and game.GetFramesElapsed() % anim_info.fci == 0) {
        if (anim_info.fps > 0)
            SetCurrFrame(anim_info.curr_frame + 1);
        else
            SetCurrFrame(anim_info.curr_frame - 1);
    }

    //Draw the sprite
    window.draw(sprite);
}

void Animation::SetSprite(const std::string new_tex) {
    if (!texture.loadFromFile("assets/Sprites/" + new_tex + ".png")) {
        std::cerr << "Failed to load texture for " << new_tex << std::endl;
        return;
    }
    sprite.setTexture(texture);
}

void Animation::SetSpriteW(const uint new_s_w) {
    if (new_s_w > 0) {
        sf::Vector2f old_ori = GetOrigin();

        anim_info.sprite_w = new_s_w;

        if (entity)
            entity->SetBBox();
        SetOrigin(old_ori);
    }
    else {
        cout << "Sprite width must be > 0!" << endl;
        anim_info.sprite_w = 1;
    }
}

void Animation::SetSpriteH(const uint new_s_h) {
    if (new_s_h > 0) {
        sf::Vector2f old_ori = GetOrigin();

        anim_info.sprite_h = new_s_h;

        if (entity)
            entity->SetBBox();
        SetOrigin(old_ori);
    }
    else {
        cout << "Sprite height must be > 0!" << endl;
        anim_info.sprite_h = 1;
    }
}

void Animation::SetTexRow(int new_t_r, const int new_n_f) {
    //Dividing the size of the texture by the height of the sprite should ALWAYS produce a whole number
    const uint num_rows = texture.getSize().y / anim_info.sprite_h;
    while (0 > new_t_r or new_t_r >= num_rows) {
        if (new_t_r < 0)
            new_t_r += num_rows;
        else if (new_t_r >= num_rows)
            new_t_r -= num_rows;
    }
    anim_info.tex_row = new_t_r;

    //If a new number of frames is not provided, it will be assumed that the # of frames is staying the same
    if (new_n_f != -1)
        SetNumFrames(new_n_f);
}

void Animation::SetCurrFrame(int new_c_f) {
    while (new_c_f < 0 or new_c_f >= anim_info.num_frames) {
        if (new_c_f < 0)
            new_c_f += anim_info.num_frames;
        else if (new_c_f >= anim_info.num_frames)
            new_c_f -= anim_info.num_frames;
    }
    anim_info.curr_frame = new_c_f;
}

void Animation::SetFPS(const int new_fps) {
    anim_info.fps = new_fps;
    if (anim_info.fps > 0)
        anim_info.fci = game.GetFPS() / anim_info.fps;
    else if (anim_info.fps < 0)
        anim_info.fci = game.GetFPS() / (anim_info.fps * -1);
    anim_info.fci = std::round(anim_info.fci);

    if (anim_info.fps != 0)
        anim_info.anim_length = anim_info.num_frames / anim_info.fps;
    else anim_info.anim_length = 0;
}

void Animation::SetOrigin(sf::Vector2f new_ori) {
    //Origin/anchor needs to be set based on sprite w/h before scaling is applied - in other words, the literal, actual w/h of the png
    //Use get local bounds?
    if (new_ori.x > 1.f)
        new_ori.x = anim_info.sprite_w * .5;
    else new_ori.x *= anim_info.sprite_w;

    if (new_ori.y > 1.0f)
        new_ori.y = anim_info.sprite_h * .5;
    else new_ori.y *= anim_info.sprite_h;

    sprite.setOrigin(new_ori);
}