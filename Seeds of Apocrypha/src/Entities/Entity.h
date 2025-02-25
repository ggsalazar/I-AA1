#pragma once
#include <iostream>
#include "../Engine/Game.h"
#include "../Engine/Scene.h"
#include "../Utils/Util.h"
#include "../Utils/Text.h"
#include "../Utils/Collision.h"
#include "../Utils/Animation.h"
#include "../Utils/Input.h"
#include "../Utils/Enums.h"

using namespace std;

struct Engine {
    Game& game;
    sf::RenderWindow& window;
    //This allows the Scene of an object to be changed
    Scene* scene;
};

class Entity : public enable_shared_from_this<Entity> {
    friend class Animation;
public:
    //Variables
    uint w = 0;
    uint h = 0;
    bool alive = true;
    int dfc = 0; //Distance from camera; draw order, basically - the lower the number, the closer to the camera

    //SFX Stuff
    sf::SoundBuffer sb;
    sf::Sound sound;

    //Engine MUST be strictly inilitiazed, but this should have been the case anyway so it's a non-issue
    Entity(const Engine& s, const AnimInfo& a_i, const Animation::Transform& t = {}, int init_dfc = 0);
    virtual ~Entity() = default;

    void SetBBox();

    virtual void GetInput() {}
    virtual void Update() {}
    virtual void Draw(const bool debug = false);
    virtual void Move(sf::Vector2f offset);
    virtual void MoveTo(sf::Vector2f new_pos);

    sf::Vector2f GetPos() const { return pos; }
    sf::FloatRect GetBBox() const { return bbox; }

    void SetScene(Scene* new_scn) { structure.scene = new_scn; }

    void PlaySound();

protected:
    sf::Vector2f pos;
    sf::RectangleShape pos_debug;
    sf::FloatRect bbox;
    sf::RectangleShape bbox_debug;

    Engine structure;

    unique_ptr<Animation> anim;
};