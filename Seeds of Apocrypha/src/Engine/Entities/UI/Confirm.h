#pragma once
#include "Button.h"

class Confirm : public UI {
public:
    Confirm(const Engine e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style,
        const string conf_str = "DEFAULT", const uint init_ui_layer = 1, const int init_dfc = 0);

    void Draw() override;
};