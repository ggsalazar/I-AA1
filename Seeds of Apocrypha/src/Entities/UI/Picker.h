#pragma once
#include <vector>
#include "UI.h"

class Picker : public UI {
public:
    Picker(Menu& m, const Sprite::Info& s_i, const UIElems e,
        const uint init_ui_layer = 0);

    void GetInput() override;
    void Draw() override;
    inline void MoveBy(Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vector2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

    void SetPicking(const string new_p);
    inline string GetPicking() const { return picking.info.str; }

private:
    //Variables
    Rect l_bbox;
    bool l_primed = false;
    Rect r_bbox;
    bool r_primed = false;

    Text picking;
    vector<string> options;
    vector<string>::iterator option_picked;

    //Functions
    void Move() override;

    inline bool LeftSelected() { return Collision::Point(Input::MousePos(), l_bbox); }
    inline void LeftPressed() { l_primed = true; }
    void LeftReleased();

    inline bool RightSelected() { return Collision::Point(Input::MousePos(), r_bbox); }
    inline void RightPressed() { r_primed = true; }
    void RightReleased();
};