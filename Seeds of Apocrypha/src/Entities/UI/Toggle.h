#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    bool on = false;

    Toggle(Game* g, Scene* s, Menu& m, const Sprite::Info& s_i, const UIElems e,
        const uint init_ui_layer = 0);

    void Draw() override;
    void MoveBy(Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    void MoveTo(Vector2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    //Functions
    void Move() override;
    //void Pressed() override;
    void Released() override;
};