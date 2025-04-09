#pragma once
#include <iostream>
#include "../Core/Collision.h"
#include "../Core/Enums.h"
#include "../Core/Geometry.h"
#include "../Core/Input.h"
#include "../Graphics/Animation.h"
#include "../Graphics/Text.h"

using namespace std;

namespace Engine {

class Game;
class Scene;

struct Engine {
    Game& game;
    RenderWindow& window;
    //This allows the Scene of an object to be changed
    Scene* scene;
};

class Entity : public enable_shared_from_this<Entity> {
    friend class Animation;
public:
    //Variables
    Vector2u size = { 0, 0 };
    bool alive = true;
    int dfc = 0; //Distance from camera; draw order, basically - the lower the number, the closer to the camera

    //SFX Stuff
    //sf::SoundBuffer sb;
    //sf::Sound sound;


    Entity(const Engine& e, const AnimInfo& a_i, const Animation::Transform& t = {}, int init_dfc = 0);
    virtual ~Entity() = default;


    virtual void GetInput() {}
    virtual void Update() {}
    virtual void Draw();
    virtual void MoveBy(Vector2i offset) { pos.x += offset.x; pos.y += offset.y; Entity::Move(); }
    virtual void MoveTo(Vector2u new_pos) { pos = new_pos; Entity::Move(); }

    Vector2u GetPos() const { return pos; }
    Rect GetBBox() const { return bbox; }
    void SetBBox();

    void SetScene(Scene* new_scn) { engine.scene = new_scn; }

    void PlaySound();

protected:
    Vector2u pos;
    Circle pos_debug;
    Rect bbox;

    Engine engine;

    unique_ptr<Animation> anim;

private:
    virtual void Move() { anim->sprite.setPosition(Vector2f(pos)); SetBBox(); }
};
}