#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    Toggle(const Engine& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {}, const int init_dfc = 0) :
        UI(s, m, a_i, t, style, init_dfc) {

        label_offset = engine.game.GetResScale()*10;
        Text::SetOrigin(label, { 1.f, .5f });
        label.setPosition({ pos.x - label_offset, pos.y });
    }

    void Draw() override;
    void Move(sf::Vector2f offset) override;
    void MoveTo(sf::Vector2f new_pos) override;

    bool GetOn() const { return on; }

private:
    bool on = false;

    //Functions
    //void Pressed() override;
    void Released() override;
};