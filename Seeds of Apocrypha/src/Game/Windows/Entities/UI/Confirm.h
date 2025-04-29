#pragma once
#include "Button.h"

class Confirm : public UI {
public:
    Confirm(Game& g, Scene* s, Menu& m, const Sprite::Info& a_i, const UI::Style& style,
        const string conf_str = "DEFAULT", const uint init_ui_layer = 1);

    void Draw() override;
};