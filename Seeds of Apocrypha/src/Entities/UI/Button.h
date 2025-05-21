#pragma once
#include "UI.h"

class Button : public UI {
public:
    Button(Menu& m, const Sprite::Info& s_i, const UIElems e,
        const uint init_ui_layer = 0)
        : UI(m, s_i, e, init_ui_layer) {

        label_offset = game->GetResScale();
        label.MoveTo({ pos.x, pos.y - label_offset });
    }

    void Draw() override;
    inline void MoveBy(Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vector2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override;
    void Pressed() override;
    void Released() override;
};