#pragma once
#include "UI.h"

class Slider : public UI {
public:
    Slider(const Engine& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {}, const int init_dfc=0);

    void GetInput() override;
    void Draw() override;
    void Move(sf::Vector2f offset) override;
    void MoveTo(sf::Vector2f new_pos) override;

private:
    //Knob stuff
    sf::Image knob_img;
    sf::Texture knob_tex;
    sf::Sprite knob_spr;
    sf::Text knob_val;
    float knob_pos_max = 0;
    float knob_pos_min = 0;
    float knob_pos = 0;
};