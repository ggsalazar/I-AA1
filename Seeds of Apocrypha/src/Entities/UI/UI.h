#pragma once
#include "../Entity.h"
#include "../../Engine/Menu.h"

class UI : public Entity {
public:
    struct Style {
        UIElems elem = UIElems::DEFAULT;
        uint font_size = 36;
    };

    Menu& menu;
    uint ui_layer;
    sf::Text label;

    UI(const Engine& e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {},
        const uint init_ui_layer = 0, const int init_dfc = 0);

    virtual void GetInput() override;
    virtual void Draw() override {}
    virtual void MoveBy(sf::Vector2f offset) override {}
    virtual void Resize(uint r_s);

    void SetActive(const bool new_active = true);
    bool GetActive() const { return active; }
    bool GetActivated() const { return activated; }
    UIElems GetElem() const { return elem; }

protected:

    sf::RectangleShape label_debug;

    //Variables
    int label_offset = 0;
    UIElems elem = UIElems::DEFAULT;
    bool active = true;
    bool primed = false;
    bool activated = false;

    //Functions
    bool Selected(const sf::Vector2i& mouse_pos) { return ui_layer == engine.game.curr_ui_layer and bbox.contains(sf::Vector2f(mouse_pos)); }
    virtual void Pressed();
    virtual void Released() {}
};