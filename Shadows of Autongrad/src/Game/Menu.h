#pragma once
#include <unordered_map>
#include "Game.h"
#include "../Core/Enums.h"
#include "../Core/Vector2.h"
#include "../Graphics/Text.h"

using namespace std;

class Scene;
class UI;

class Menu {
public:
    Menu(const Menus init_label);
    virtual ~Menu() {
        for (auto [_, e] : ui_elems) {
            delete e;
            e = nullptr;
        }
        ui_elems.clear();

        for (auto [_, sm] : sub_menus) {
            delete sm;
            sm = nullptr;
        }
        sub_menus.clear();
    }
    static inline void SetGame(Game* g) { game = g; }

    //Engine
    virtual void GetInput();
    virtual void Update();
    virtual void Draw();

    //Self and Sub-Menus
    void Open(const bool o = true);
    inline void SetSupStr(const string& new_str) { sup_text.info.str = new_str; }
    void Resize();
    inline Menus GetLabel() const { return label; }
    inline bool GetOpen() const { return open; }
    void OpenSM(const Menus s_m);

    //UI Elements
    void RemoveUIElem(const UIElem ui);
    bool CheckUIElem(const UIElem ui);
    bool GetUIElemActive(const UIElem ui);
    void SetUIElemActive(const UIElem ui, const bool a = true);
    void SetUIElemStatus(const UIElem ui, const string new_status);
    string GetUIElemStatus(const UIElem ui);

protected:
    Menus label;

    Text menu_text;
    Text sup_text; //Supplementary text
    bool open = false;
    uint res_scalar;

    unordered_map<Menus, Menu*> sub_menus;
    unordered_map<UIElem, UI*> ui_elems;
    Vector2f ui_ori = { .5f, .5f };
    Vector2i ui_scale = { 1, 1 };

    //Pointers
    static inline Game* game;
    Scene& scene;
};