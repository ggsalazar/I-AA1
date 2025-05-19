#pragma once
#include "UI.h"

class Button : public UI {
public:
    Button(Game* g, Scene* s, Menu& m, const Sprite::Info& s_i, const UIElems e,
        const uint init_ui_layer = 0)
        : UI(g, s, m, s_i, e, init_ui_layer) {

        label_offset = game->GetResScale();
        label.MoveTo({ pos.x, pos.y - label_offset });
    }

    void Draw() override;
    void MoveBy(Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    void MoveTo(Vector2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override;
    void Pressed() override;
    void Released() override;
};