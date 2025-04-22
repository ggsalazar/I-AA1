#pragma once
#include <vector>
#include "Game.h" //Includes all engine stuff
//#include "TileMap.h"

//Forward declarations
class Menu;
class PartyMember;

class Scene {
public:
    Scenes label;
    bool selecting = false;
    Interfaces interface_open = Interfaces::NONE;

    Scene(Game& g, Scenes init_label) :
        game(g), label(init_label) {}

    //Engine stuff
    void GetInput();
    void Update();
    void Draw();

    //Game functions
    void OpenInterface(Interfaces intrfc = Interfaces::NONE);
    void MoveCamera();
    void SelectPartyMems();
    Actions LMBAction();

    //Scene handling
    void Open(const bool o = true);
    bool IsOpen() const { return open; }
    
    //Menus
    void OpenMenu(Menus menu, const bool o = true);
    bool MenuOpen(Menus menu);
    void ResizeMenus();
    
    //Entities
    void AddEntity(shared_ptr<Entity> e) { entities.push_back(e); }
    void RemoveEntity(shared_ptr<Entity> e);
    void RemoveEntity(const string ent_name);
    void SetEntitySFXVolume(const float new_volume);

    //Party Members
    void CreatePartyMem();
    void CreatePreGen(PreGens p_g);
    vector<shared_ptr<PartyMember>> GetPartyMems() const { return party_mems; }
    void SetPartyMems(vector<shared_ptr<PartyMember>> p_ms) { party_mems = p_ms; }

private:
    bool open = false;
    Actions action = Actions::DEFAULT;

    //TileMap tilemap;

    //unordered_map<Menus, unique_ptr<Menu>> menus;
    vector<shared_ptr<Entity>> entities;
    vector<shared_ptr<PartyMember>> party_mems;


    Rect selec_box;
    Vector2f selec_area = { 0, 0 };
    //queue<Vector2u> found_path;

    //Game pointer
    Game& game;
};