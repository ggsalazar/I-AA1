#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    bool on = false;

    Toggle(Menu& m, const Sprite::Info& s_i, const UIElem e,
        const uint init_ui_layer = 0);

    void Draw() override;
    inline void MoveBy(Vector2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vector2f new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    //Functions
    void Move() override;
    //void Pressed() override;
    void Released() override;
};