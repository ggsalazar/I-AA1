#pragma once
#include "../Entity.h" //Collision (Geometry (Vector2 (iostream))), Sprite, Text, Game, Scene
#include "../../Game/Menu.h"
#include "../../Core/Input.h" //(Window)

class UI : public Entity {
public:
    Menu& menu;
    uint ui_layer;
    Text label;

    UI(Menu& m, const Sprite::Info& s_i, const UIElem e,
        const uint init_ui_layer = 0);

    virtual void GetInput() override;
    inline virtual void Draw() override {};

    virtual void Resize(uint r_s);

    inline bool Selected() { return active and Collision::RectPoint(bbox, Input::MousePos()); }

    void SetActive(const bool new_active = true);
    inline bool GetActive() const { return active; }
    inline bool GetActivated() const { return activated; }
    inline UIElem GetElem() const { return elem; }

protected:
    //Variables
    int label_offset = 0;
    UIElem elem = UIElem::DEFAULT;
    bool active = true;
    bool primed = false;
    bool activated = false;

    //Functions
    virtual void Pressed();
    virtual void Released() {}
};