#pragma once
#include <queue>
#include <unordered_map>
#include "../Core/Geometry.h"
#include "../Core/Pathfinding.h"
#include "../Graphics/TileMap.h" //nlohmann/json

using namespace std;

class Game;
class Menu;
class Interface;
class Entity;
class Creature;
class PartyMember;

class Scene {
public:
    Scenes label;
    bool selecting = false; //Whether we are selecting party members
    bool lmb_action = true; //Whether we are using the default action
    Menus interface_open = Menus::NOINTRFC;

    Scene() = default;
    Scene(Scenes init_label) : label(init_label) {}
    ~Scene() {
        for (auto e : entities) {
            delete e;
            e = nullptr;
        }
        entities.clear();

        for (auto [_, m] : menus) {
            delete m;
            m = nullptr;
        }
        menus.clear();
    }
    void Open(const bool o = true);

    static inline void SetGame(Game* g) { game = g; }
    inline bool IsOpen() const { return open; }

    //Engine stuff
    void GetInput();
    void Update();
    void Draw();

    //Menu handling
    void OpenMenu(Menus menu, const bool o = true);
    bool MenuOpen(Menus menu);
    void ResizeMenus();

    //Gameplay functions
    void OpenInterface();
    void MoveCamera();
    void SelectPartyMems();
    Action LMBAction(vector<PartyMember*>& s_pms);
    void SetGameCursor(Action action = Action::DEFAULT);
    void PerformAction();

    //Entities
    inline void AddEntity(Entity* e) { entities.push_back(e); }
    void RemoveEntity(Entity* e);
    void SetEntitySFXVolume(const float new_volume);

    //Party Members
    void CreatePartyMem();
    void CreatePreGen(PreGens p_g);
    inline vector<PartyMember*> GetPartyMems() const { return party_mems; }
    inline void SetPartyMems(vector<PartyMember*> p_ms) { party_mems = p_ms; }
    float CalcPartyAeons();

    //NPCs
    void LoadNPCs();

    //Dialogue
    void LoadDialogue();
    void inline EndDialogue() { in_dlg = false; }

private:
    bool open = false;
    Action action = Action::DEFAULT;
    Action held_action = Action::NOACTION;
    Entity* mouse_tar = nullptr;
    Entity* held_tar = nullptr;
    Creature* cam_tar = nullptr;

    bool in_dlg = false;

    TileMap tilemap;
    Pathfinding grid;

    unordered_map<Menus, Menu*> menus;
    vector<Entity*> entities;
    vector<PartyMember*> party_mems;

    //Edge panning rects
    Rect up_edge;
    Rect down_edge;
    Rect left_edge;
    Rect right_edge;

    Rect selec_box;
    array<vector<Vector2i>, 4> found_paths;

    inline static Game* game = nullptr;
};