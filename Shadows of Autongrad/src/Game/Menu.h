#pragma once
#include <unordered_map>
#include "../Core/Aliases.h"
#include "../Core/Enums.h"
#include "../Core/Vector2.h"
#include "../Graphics/Text.h"

using namespace std;

class Game;
class Scene;
class UI;

class Menu {
public:
    Menu(const Menus init_label);
    ~Menu() {
        for (auto [_, sm] : sub_menus) {
            delete sm;
            sm = nullptr;
        }
    }
    virtual ~Menu() {}
    static inline void SetGame(Game* g) { game = g; }

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
    unordered_map<UIElem, s_ptr<UI>> ui_elems;
    Vector2f ui_ori = { .5f, .5f };
    Vector2i ui_scale = { 1, 1 };

    //Pointers
    static inline Game* game;
    Scene& scene;
};