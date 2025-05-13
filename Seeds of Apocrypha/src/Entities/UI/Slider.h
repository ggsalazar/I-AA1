#pragma once
#include "UI.h"

class Slider : public UI {
public:
    Slider(Game& g, Scene* s, Menu& m, const Sprite::Info& s_i, const UIElems e,
        const uint init_ui_layer = 0);

    void GetInput() override;
    void Draw() override;
    void MoveBy(Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    void MoveTo(Vector2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override;

    //Knob stuff
    u_ptr<Sprite> knob_spr;
    u_ptr<Text> knob_label;
    float knob_pos_max = 0;
    float knob_pos_min = 0;
    float knob_pos = 0;
};