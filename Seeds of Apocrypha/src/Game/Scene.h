#pragma once
#include <queue>
#include <unordered_map>
#include "../Core/Geometry.h"
#include "../Core/Pathfinding.h" //Aliases
#include "../Graphics/TileMap.h" //nlohmann/json

using namespace std;

class Game;
class Menu;
class Entity;
class PartyMember;

class Scene {
public:
    Scenes label;
    bool selecting = false;
    //Are we using the default action at the moment? We aren't when:
    // -Clicking UI elements (including all creature portraits)
    // -The game is paused
    // -No party members are selected
    bool lmb_action = true;
    Interfaces interface_open = Interfaces::NONE;

    Scene() = default;
    Scene(Scenes init_label) : label(init_label) {}
    ~Scene() {
        for (auto [_, m] : menus) {
            delete m;
            m = nullptr;
        }
    }
    static inline void SetGame(Game* g) { game = g; }
    void Open(const bool o = true);
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
    void OpenInterface(Interfaces intrfc = Interfaces::NONE);
    void MoveCamera();
    void SelectPartyMems();
    Actions LMBAction();
    void SetGameCursor(Actions action = Actions::DEFAULT);

    //Entities
    inline void AddEntity(s_ptr<Entity> e) { entities.push_back(e); }
    void RemoveEntity(s_ptr<Entity> e);
    void RemoveEntity(const string ent_name);
    void SetEntitySFXVolume(const float new_volume);

    //Party Members
    void CreatePartyMem();
    void CreatePreGen(PreGens p_g);
    inline vector<s_ptr<PartyMember>> GetPartyMems() const { return party_mems; }
    inline void SetPartyMems(vector<s_ptr<PartyMember>> p_ms) { party_mems = p_ms; }

    //NPCs
    void CreateNPC();

private:
    bool open = false;
    Actions action = Actions::DEFAULT;

    TileMap tilemap;
    Pathfinding grid;

    unordered_map<Menus, Menu*> menus;
    vector<s_ptr<Entity>> entities;
    vector<s_ptr<PartyMember>> party_mems;

    //Edge panning rects
    Rect up_edge;
    Rect down_edge;
    Rect left_edge;
    Rect right_edge;

    Rect selec_box;
    queue<Vector2i> found_path;

    inline static Game* game = nullptr;
};