#pragma once
#include "UI.h"

class Slider : public UI {
public:
    Slider(const Structure& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {}, const int init_dfc=0);

    void GetInput() override;
    void Draw(const bool debug = false) override;
    void Move(sf::Vector2f offset) override;
    void MoveTo(sf::Vector2f new_pos) override;
    void Resize(uint r_s) override { UI::Resize(r_s); anim->sprite.setScale({ r_s * 3.f, r_s * 2.f }); } //In the future, draw your sliders so they don't need their own special setScale rules

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