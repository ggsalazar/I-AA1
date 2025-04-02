#pragma once
#include <vector>
#include "UI.h"


class Picker : public UI {
public:
    Picker(const Engine& e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {},
        const uint init_ui_layer = 0, const int init_dfc=0);

    void GetInput() override;
    void Draw() override;
    void MoveBy(sf::Vector2i offset) override { Entity::MoveBy(offset); Move(); }
    void MoveTo(sf::Vector2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

    void SetPicking(const string new_p);
    string GetPicking() { return picking.getString(); }

private:
    //Variables
    sf::FloatRect l_bbox;
    sf::RectangleShape l_bbox_debug;
    bool l_primed = false;
    sf::FloatRect r_bbox;
    sf::RectangleShape r_bbox_debug;
    bool r_primed = false;

    sf::Text picking;
    vector<string> options;
    vector<string>::iterator option_picked;

    //Functions
    void Move() override;

    bool LeftSelected(const sf::Vector2i& mouse_pos) { return l_bbox.contains(sf::Vector2f(mouse_pos)); }
    void LeftPressed() { l_primed = true; }
    void LeftReleased();

    bool RightSelected(const sf::Vector2i& mouse_pos) { return r_bbox.contains(sf::Vector2f(mouse_pos)); }
    void RightPressed() { r_primed = true; }
    void RightReleased();
};