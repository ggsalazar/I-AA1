#pragma once
#include <iostream>
#include "../Engine/Utils/Input.h"
#include "../Engine/Utils/Enums.h"
#include "../Engine/Game.h"
#include "../Engine/Scene.h"
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


    Entity(const Engine& e, const AnimInfo& a_i, const Animation::Transform& t = {}, int init_dfc = 0);
    virtual ~Entity() = default;


    virtual void GetInput() {}
    virtual void Update() {}
    virtual void Draw();
    virtual void MoveBy(sf::Vector2i offset) { pos += offset; Entity::Move(); }
    virtual void MoveTo(sf::Vector2i new_pos) { pos = new_pos; Entity::Move(); }

    sf::Vector2i GetPos() const { return pos; }
    sf::FloatRect GetBBox() const { return bbox; }
    void SetBBox();

    void SetScene(Scene* new_scn) { engine.scene = new_scn; }

    void PlaySound();

protected:
    sf::Vector2i pos;
    sf::RectangleShape pos_debug;
    sf::FloatRect bbox;
    sf::RectangleShape bbox_debug;

    Engine engine;

    unique_ptr<Animation> anim;

private:
    virtual void Move() { sf::Vector2f s_pos = sf::Vector2f(pos); anim->sprite.setPosition(s_pos); SetBBox(); }
};