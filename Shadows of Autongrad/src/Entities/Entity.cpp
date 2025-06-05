#include "Entity.h"

#undef PlaySound

Entity::Entity(const Sprite::Info& s_i)
    : scene(game->active_scene), pos(s_i.pos), float_pos(pos), pos_debug(pos, 4) /*sound(sb)*/ {
    sprite.Init(s_i);
    Entity::MoveTo(float_pos);
}

void Entity::Draw() {
    game->renderer.DrawSprite(sprite);
}

void Entity::MoveBy(Vector2f offset) {
    float_pos += offset;

    Entity::Move();
}

void Entity::MoveTo(Vector2f new_pos) {
    float_pos = new_pos;

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
    pos = Round(float_pos);

    sprite.MoveTo(float_pos);
    size = sprite.GetSprSize();

    pos_debug.x = pos.x;
    pos_debug.y = pos.y;

    //bbox position will always be top left
    bbox.x = pos.x - sprite.GetOrigin().x * size.x;
    bbox.y = pos.y - sprite.GetOrigin().y * size.y;
    bbox.w = size.x;
    bbox.h = size.y;
}
