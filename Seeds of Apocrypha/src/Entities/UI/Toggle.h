#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    bool on = false;

    Toggle(const Engine& e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {},
        const uint init_ui_layer=0, const int init_dfc = 0);

    void Draw() override;
    void MoveBy(sf::Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    void MoveTo(sf::Vector2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    //Functions
    void Move() override;
    //void Pressed() override;
    void Released() override;
};