#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "Utils/Enums.h"
#include "Utils/Macros.h"

using namespace std;

//Forward declarations
class Game;
class Scene;
class UI;

class Menu {
public:
    //Variables
    Menus label;

    Menu(Game& g, sf::RenderWindow& w, Scene& s, Menus init_label);

    //Engine
    void Update();
    void Draw();

    //Self and Sub-Menus
    void Open(const bool o = true);
    bool GetOpen() const { return open; }
    void OpenSM(const Menus s_m);

    //UI Elements
    void Resize();
    //LastSelected();
    bool CheckUIElem(const UIElems ui);
    bool GetUIElemActive(const UIElems ui);
    void SetUIElemActive(const UIElems ui, const bool a = true);
    void SetUIElemStatus(const UIElems ui, const string new_status);
    string GetUIElemStatus(const UIElems ui);

private:
    sf::Text menu_text;
    sf::Text sup_text; //Supplementary text
    bool open = false;
    uint res_scalar;
    uint style_size;

    unordered_map<Menus, unique_ptr<Menu>> sub_menus;
    unordered_map<UIElems, shared_ptr<UI>> ui_elems;
    sf::Vector2f ui_ori = { .5f, .5f };
    sf::Vector2u ui_size = { 1, 1 };

    //Engine pointers
    Game& game;
    sf::RenderWindow& window;
    Scene& scene;
};