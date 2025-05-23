#pragma once
#include "../Core/Collision.h" //Geometry (Vector2 (iostream))
#include "../Graphics/Sprite.h"
#include "../Graphics/Text.h"
#include "../Game/Game.h"
#include "../Game/Scene.h"

class Entity {
public:
    //Variables
    Vector2i size = { 0, 0 };
    bool alive = true;

    Sprite sprite;

    //SFX Stuff
    //sf::SoundBuffer sb;
    //sf::Sound sound;

    Entity(const Sprite::Info& s_i);
    virtual ~Entity() = default;

    static inline void SetGame(Game* g) { game = g; }

    virtual void GetInput() {}
    inline virtual void Update() {}
    virtual void Draw();
    virtual void MoveBy(Vector2i offset);
    virtual void MoveTo(Vector2i new_pos);

    inline Vector2i GetPos() const { return pos; }
    inline Rect GetBBox() const { return bbox; }

    inline void SetScene(Scene* new_scn) { scene = new_scn; }

    void PlaySound();

protected:
    Vector2i pos;
    Circle pos_debug;
    Rect bbox;

    inline static Game* game = nullptr;
    Scene* scene = nullptr;

    virtual void Move();
};