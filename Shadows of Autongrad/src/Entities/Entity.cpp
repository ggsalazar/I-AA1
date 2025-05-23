#include "Entity.h"

#undef PlaySound

Entity::Entity(const Sprite::Info& s_i)
    : scene(game->active_scene), pos(s_i.pos), pos_debug(pos, 4) /*sound(sb)*/ {
    sprite.Init(s_i);
    Entity::MoveTo(pos);
}

void Entity::Draw() {
    game->renderer.DrawSprite(sprite);
}

void Entity::MoveBy(Vector2i offset) {
    pos += offset;

    Entity::Move();
}

void Entity::MoveTo(Vector2i new_pos) {
    pos = new_pos;

    Entity::Move();
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

void Entity::Move() {
    sprite.MoveTo(pos);
    size = sprite.GetSprSize();

    pos_debug.x = pos.x;
    pos_debug.y = pos.y;

    //bbox position will always be top left
    bbox.x = pos.x - sprite.GetOrigin().x * size.x;
    bbox.y = pos.y - sprite.GetOrigin().y * size.y;
    bbox.w = size.x;
    bbox.h = size.y;
}
