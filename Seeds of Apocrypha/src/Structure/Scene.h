#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "../Utils/Enums.h"
#include "TileMap.h"
#include "Menu.h"

//Forward declarations
class Game;
class Entity;
class PartyMember;

class Scene {
public:
    Scenes label;

    Scene(Game& g, sf::RenderWindow& win, Scenes init_label) :
        game(g), window(win), label(init_label) {}

    //General game stuff
    void GetInput();
    void Update();
    void Draw();
    void MoveCamera();

    //Scene handling
    void Open();
    void Close();
    bool IsOpen() const { return open; }
    
    //Menus
    void OpenMenu(Menus menu);
    bool MenuOpen(Menus menu);
    void CloseMenu(Menus menu);
    void ResizeMenus();
    
    //Entities
    void AddEntity(shared_ptr<Entity> e) { entities.push_back(e); }

    //Party Members
    void CreatePartyMember();
    void CreatePreGen(PreGens p_g);
    vector<shared_ptr<PartyMember>> GetPartyMems() const { return party_mems; }
    void SetPartyMems(vector<shared_ptr<PartyMember>> p_ms) { party_mems = p_ms; }

private:
    bool open = false;

    TileMap tilemap;

    unordered_map<Menus, unique_ptr<Menu>> menus;
    vector<shared_ptr<Entity>> entities;
    vector<shared_ptr<PartyMember>> party_mems;

    //Game structure stuff
    Game& game;
    sf::RenderWindow& window;
};