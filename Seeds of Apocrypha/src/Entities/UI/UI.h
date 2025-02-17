#pragma once
#include "../Entity.h"
#include "../../Structure/Menu.h"

class UI : public Entity {
public:
    struct Style {
        UIElems elem = UIElems::DEFAULT;
        int font_size = 36;
    };

    Menu& menu;

    sf::Text label;

    UI(const Structure& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {}, const int init_dfc = 0);

    virtual void GetInput() override;
    virtual void Draw(const bool debug = false) override {}
    virtual void Move(sf::Vector2f offset) override {}
    virtual void Resize(uint r_s);

    void SetActive(const bool new_active = true);
    bool GetActive() const { return active; }
    bool GetActivated() const { return activated; }

protected:
    //Variables
    int label_offset = 0;
    UIElems elem = UIElems::DEFAULT;
    bool active = true;
    bool primed = false;
    bool activated = false;

    //Functions
    bool Selected(const sf::Vector2i& mouse_pos) { return bbox.contains(sf::Vector2f(mouse_pos)); }
    virtual void Pressed();
    virtual void Released() {}
};