#pragma once
#include "UI.h"

class Button : public UI {
public:
    Button(const Engine& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {},
        const uint init_ui_layer = 0, const int init_dfc=0) :
        UI(s, m, a_i, t, style, init_ui_layer, init_dfc) {

        label_offset = engine.game.GetResScale();
        label.setPosition(Vector2f(pos.x, pos.y - label_offset));
    }

    void Draw() override;
    void MoveBy(Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    void MoveTo(Vector2u new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    //Functions
    void Move() override;
    void Pressed() override;
    void Released() override;
};