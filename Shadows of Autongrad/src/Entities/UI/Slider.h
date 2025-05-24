#pragma once
#include "UI.h"

class Slider : public UI {
public:
    Slider(Menu& m, const Sprite::Info& s_i, const UIElem e,
        const uint init_ui_layer = 0);

    void GetInput() override;
    void Draw() override;
    inline void MoveBy(Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vector2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override;

    //Knob stuff
    Sprite knob_spr;
    Text knob_label;
    int knob_pos_max = 0, knob_pos_min = 0;
    int knob_pos = 0;
};