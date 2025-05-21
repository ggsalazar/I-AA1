#pragma once
#include "../Entity.h" //Collision (Geometry (Vector2 (iostream))), Sprite, Text, Game, Scene
#include "../../Game/Menu.h"
#include "../../Core/Input.h" //(Window)

class UI : public Entity {
public:
    Menu& menu;
    uint ui_layer;
    Text label;

    UI(Menu& m, const Sprite::Info& s_i, const UIElems e,
        const uint init_ui_layer = 0);

    virtual void GetInput() override;
    virtual void Resize(uint r_s);

    void SetActive(const bool new_active = true);
    inline bool GetActive() const { return active; }
    inline bool GetActivated() const { return activated; }
    inline UIElems GetElem() const { return elem; }

protected:
    //Variables
    int label_offset = 0;
    UIElems elem = UIElems::DEFAULT;
    bool active = true;
    bool primed = false;
    bool activated = false;

    //Functions
    inline bool Selected() { return Collision::Point(Input::MousePos(), bbox); }
    virtual void Pressed();
    virtual void Released() {}
};