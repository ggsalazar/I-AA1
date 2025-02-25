#pragma once
#include <iostream>
#include "../Engine/Utils/Input.h"
#include "../Engine/Utils/Enums.h"
#include "../Engine/Game.h"
#include "../Engine/Scene.h"
#include "../Engine/Math.h"
#include "../Engine/Text.h"
#include "../Engine/Collision.h"
#include "../Engine/Animation.h"

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
    Entity(const Engine& e, const AnimInfo& a_i, const Animation::Transform& t = {}, int init_dfc = 0);
    virtual ~Entity() = default;


    virtual void GetInput() {}
    virtual void Update() {}
    virtual void Draw();
    virtual void Move(sf::Vector2f offset);
    virtual void MoveTo(sf::Vector2f new_pos);

    sf::Vector2f GetPos() const { return pos; }
    sf::FloatRect GetBBox() const { return bbox; }
    void SetBBox();

    void SetScene(Scene* new_scn) { engine.scene = new_scn; }

    void PlaySound();

protected:
    sf::Vector2f pos;
    sf::RectangleShape pos_debug;
    sf::FloatRect bbox;
    sf::RectangleShape bbox_debug;

    Engine engine;

    unique_ptr<Animation> anim;
};