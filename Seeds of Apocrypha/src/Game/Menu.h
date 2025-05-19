#pragma once
#include "Game.h" //(<unordered_map>, <iostream>, and aliases)

//class Game;
//class Scene;
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
    inline Menus GetLabel() const { return label; }
    inline bool GetOpen() const { return open; }
    void OpenSM(const Menus s_m);

    //UI Elements
    void AddUIElem(const s_ptr<UI> new_elem);
    void RemoveUIElem(const UIElems ui);
    bool CheckUIElem(const UIElems ui);
    bool GetUIElemActive(const UIElems ui);
    void SetUIElemActive(const UIElems ui, const bool a = true);
    void SetUIElemStatus(const UIElems ui, const string new_status);
    string GetUIElemStatus(const UIElems ui);

protected:
    Menus label;

    Text menu_text;
    Text sup_text; //Supplementary text
    bool open = false;
    uint res_scalar;

    unordered_map<Menus, u_ptr<Menu>> sub_menus;
    unordered_map<UIElems, s_ptr<UI>> ui_elems;
    Vector2f ui_ori = { .5f, .5f };
    Vector2i ui_scale = { 1, 1 };

    //Engine pointers
    Game& game;
    Scene& scene;
};