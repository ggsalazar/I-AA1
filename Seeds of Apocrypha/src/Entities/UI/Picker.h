#pragma once
#include <vector>
#include "UI.h"


class Picker : public UI {
public:
    Picker(const Structure& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {}, const int init_dfc=0);

    void GetInput() override;
    void Draw(const bool debug = false) override;
    void Move(sf::Vector2f offset) override;
    void MoveTo(sf::Vector2f new_pos) override;

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
    bool LeftSelected(const sf::Vector2i& mouse_pos) { return l_bbox.contains(sf::Vector2f(mouse_pos)); }
    void LeftPressed() { l_primed = true; }
    void LeftReleased();

    bool RightSelected(const sf::Vector2i& mouse_pos) { return r_bbox.contains(sf::Vector2f(mouse_pos)); }
    void RightPressed() { r_primed = true; }
    void RightReleased();
};