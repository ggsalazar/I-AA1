#pragma once
#include "../../Game.h" //Includes Engine.h, which includes Entity.h
#include "../../Menu.h"

class UI : public Entity {
public:
    struct Style {
        UIElems elem = UIElems::DEFAULT;
        uint font_size = 36;
    };

    Menu& menu;
    uint ui_layer;
    u_ptr<Text> label;

    UI(Game& g, Scene* s, Menu& m, const Sprite::Info& s_i, const UI::Style& style = {},
        const uint init_ui_layer = 0);

    virtual void GetInput() override;
    virtual void Draw() override {}
    virtual void MoveBy(Vector2i offset) override {}
    virtual void Resize(uint r_s);

    void SetActive(const bool new_active = true);
    bool GetActive() const { return active; }
    bool GetActivated() const { return activated; }
    UIElems GetElem() const { return elem; }

protected:

    //Variables
    int label_offset = 0;
    UIElems elem = UIElems::DEFAULT;
    bool active = true;
    bool primed = false;
    bool activated = false;

    //Functions
    bool Selected() { return Collision::Point(Input::MousePos(), bbox); }
    virtual void Pressed();
    virtual void Released() {}
};