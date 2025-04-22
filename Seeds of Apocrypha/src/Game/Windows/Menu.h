#pragma once
#include <iostream>
#include <memory>
#include <unordered_map>
//#include "Enums.h"
//#include "Vector2.h"

using namespace std;

//Forward declarations
class Game;
class Scene;
class UI;

class Menu {
public:
    Menu(Game& g, Scene& s, const Menus init_label);

    //Engine
    void Update();
    void Draw();

    //Self and Sub-Menus
    void Open(const bool o = true);
    void Resize();
    Menus GetLabel() const { return label; }
    bool GetOpen() const { return open; }
    void OpenSM(const Menus s_m);

    //UI Elements
    void AddUIElem(const shared_ptr<UI> new_elem);
    void RemoveUIElem(const UIElems ui);
    bool CheckUIElem(const UIElems ui);
    bool GetUIElemActive(const UIElems ui);
    void SetUIElemActive(const UIElems ui, const bool a = true);
    void SetUIElemStatus(const UIElems ui, const string new_status);
    string GetUIElemStatus(const UIElems ui);

protected:
    Menus label;

    //sf::Text menu_text;
    //sf::Text sup_text; //Supplementary text
    bool open = false;
    uint res_scalar;
    uint style_size;

    unordered_map<Menus, unique_ptr<Menu>> sub_menus;
    unordered_map<UIElems, shared_ptr<UI>> ui_elems;
    Vector2f ui_ori = { .5f, .5f };
    Vector2u ui_size = { 1, 1 };

    //Engine pointers
    Game& game;
    Scene& scene;
};