#pragma once
#include "Button.h"

class Confirm : public UI {
public:
    Confirm(Game& g, Scene* s, Menu& m, const Sprite::Info& s_i, const UIElems e,
        const string conf_str = "DEFAULT", const uint init_ui_layer = 1);

    void Draw() override;
};