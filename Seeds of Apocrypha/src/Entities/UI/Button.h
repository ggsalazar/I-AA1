#pragma once
#include "UI.h"

class Button : public UI {
public:
    Button(const Engine& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {}, const int init_dfc=0) :
        UI(s, m, a_i, t, style, init_dfc) {

        label_offset = engine.game.GetResScale();
        label.setPosition({ pos.x, pos.y - label_offset });
    }

    void Draw() override;
    void Move(sf::Vector2f offset) override;
    void MoveTo(sf::Vector2f new_pos) override;

private:
    //Functions
    void Pressed() override;
    void Released() override;
};