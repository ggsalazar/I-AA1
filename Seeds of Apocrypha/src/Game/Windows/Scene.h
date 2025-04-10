#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
//#include "Enums.h"
//#include "TileMap.h"
#include "Menu.h"
//#include "Vector2.h"

//Forward declarations
class Game;
class Entity;
class PartyMember;

class Scene {
public:
    Scenes label;
    bool selecting = false;
    Interfaces interface_open = Interfaces::NONE;

    Scene(Game& g, sf::RenderWindow& win, Scenes init_label) :
        game(g), window(win), label(init_label) {}

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

    unordered_map<Menus, unique_ptr<Menu>> menus;
    vector<shared_ptr<Entity>> entities;
    vector<shared_ptr<PartyMember>> party_mems;


    sf::RectangleShape selec_box;
    sf::FloatRect selec_area;
    Vector2f selec_wh = { 0, 0 };
    //queue<Vector2u> found_path;

    //Engine stuff
    Game& game;
    sf::RenderWindow& window;
};