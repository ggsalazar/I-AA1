#pragma once
#include <iostream>
#include "../Core/Aliases.h"
#include "../Core/Collision.h"
#include "../Core/Enums.h"
#include "../Core/Geometry.h"
#include "../Graphics/Sprite.h"
#include "../Graphics/Text.h"
#include "../Game/Game.h"
#include "../Game/Scene.h"

class Game;
class Scene;

class Entity {
public:
    //Variables
    Vector2u size = { 0, 0 };
    bool alive = true;

    u_ptr<Sprite> sprite;

    //SFX Stuff
    //sf::SoundBuffer sb;
    //sf::Sound sound;

    Entity(Game& g, Scene* s, const Sprite::Info& s_i);
    virtual ~Entity() = default;


    virtual void GetInput() {}
    inline virtual void Update() {}
    virtual void Draw();
    inline virtual void MoveBy(Vector2i offset) { pos += offset; Move(); }
    inline virtual void MoveTo(Vector2i new_pos) { pos = new_pos; Move(); }

    inline Vector2i GetPos() const { return pos; }
    inline Rect GetBBox() const { return bbox; }

    inline void SetScene(Scene* new_scn) { scene = new_scn; }

    void PlaySound();

protected:
    Vector2i pos;
    Circle pos_debug;
    Rect bbox;

    Game& game;
    Scene* scene;

    virtual void Move();
};