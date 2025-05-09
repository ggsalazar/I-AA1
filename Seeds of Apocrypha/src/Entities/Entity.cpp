#include "Entity.h"
#include "../Game/Game.h"
#include "../Game/Scene.h"

#undef PlaySound

Entity::Entity(Game& g, Scene* s, const Sprite::Info& s_i) 
    : game(g), scene(s), pos(s_i.pos), pos_debug(pos, 2), bbox(pos, size) /*sound(sb)*/ {
    sprite = make_u<Sprite>(game.renderer->GetRenderer(), s_i);
    SetBBox();
}

void Entity::Draw() {
    game.renderer->DrawSprite(*sprite);
}

void Entity::SetBBox() {
    pos_debug.x = pos.x;
    pos_debug.y = pos.y;

    size = sprite->GetSprSize();

    //bbox position will always be top left
    bbox.x = pos.x - sprite->GetOrigin().x * size.x;
    bbox.y = pos.y - sprite->GetOrigin().y * size.y;
    bbox.w = size.x;
    bbox.h = size.y;
}

void Entity::PlaySound() {
    //Play assigned noise with slight pitch shift
    float pitch_shift = (rand() % 10) * .01;
    if (rand() % 2) pitch_shift *= -1;
    /*
    sound.setPitch(sound.getPitch() + pitch_shift);
    sound.play();
    sound.setPitch(1);
    */
}
