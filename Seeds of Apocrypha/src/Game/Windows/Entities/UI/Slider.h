#pragma once
#include "UI.h"

class Slider : public UI {
public:
    Slider(const Engine& e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {},
        const uint init_ui_layer=0, const int init_dfc=0);

    void GetInput() override;
    void Draw() override;
    void MoveBy(Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    void MoveTo(Vector2u new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:

    void Move() override;

    //Knob stuff
    sf::Image knob_img;
    sf::Texture knob_tex;
    sf::Sprite knob_spr;
    sf::Text knob_val;
    uint knob_pos_max = 0;
    uint knob_pos_min = 0;
    uint knob_pos = 0;
};