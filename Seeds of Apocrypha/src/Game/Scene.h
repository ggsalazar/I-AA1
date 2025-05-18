#pragma once
#include <vector>
#include <queue>
#include "Game.h" //(<iostream>, <unordered_map>, Enums.h, Input.h, Math.h, Window.h, Renderer.h)
#include "Menu.h"
#include "../Graphics/TileMap.h"

class Entity;
class PartyMember;

class Scene {
public:
    Scenes label;
    bool selecting = false;
    Interfaces interface_open = Interfaces::NONE;

    Scene() = default;
    Scene(Game& g, Scenes init_label) :
        game(g), label(init_label) {
    }

    //Engine stuff
    void GetInput();
    void Update();
    void Draw();

    //Scene handling
    void Open(const bool o = true);
    bool IsOpen() const { return open; }

    //Menu handling
    void OpenMenu(Menus menu, const bool o = true);
    bool MenuOpen(Menus menu);
    void ResizeMenus();

    //Gameplay functions
    void OpenInterface(Interfaces intrfc = Interfaces::NONE);
    void MoveCamera();
    void SelectPartyMems();
    Actions LMBAction();
    void SetGameCursor(Actions action);

    //Entities
    void AddEntity(s_ptr<Entity> e) { entities.push_back(e); }
    void RemoveEntity(s_ptr<Entity> e);
    void RemoveEntity(const string ent_name);
    void SetEntitySFXVolume(const float new_volume);

    //Party Members
    void CreatePartyMem();
    void CreatePreGen(PreGens p_g);
    vector<s_ptr<PartyMember>> GetPartyMems() const { return party_mems; }
    void SetPartyMems(vector<s_ptr<PartyMember>> p_ms) { party_mems = p_ms; }

private:
    bool open = false;
    Actions action = Actions::DEFAULT;

    TileMap tilemap;

    unordered_map<Menus, u_ptr<Menu>> menus;
    vector<s_ptr<Entity>> entities;
    vector<s_ptr<PartyMember>> party_mems;

    //Edge panning rects
    Rect up_edge;
    Rect down_edge;
    Rect left_edge;
    Rect right_edge;

    Rect selec_box;
    queue<Vector2i> found_path;

    Game& game;
};