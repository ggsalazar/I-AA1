#include "Entity.h"

Entity::Entity(const Engine& e, const AnimInfo& a_i, const Animation::Transform& t, int init_dfc) :
    engine(e), pos(t.pos), pos_debug(pos, 2),
    dfc(init_dfc), sound(sb), anim(make_unique<Animation>(engine.game, engine.window, this, a_i, t)) {

    SetBBox();
}

void Entity::SetBBox() {
    pos_debug.pos = pos;

    size = { anim->GetSpriteW() * anim->sprite.getScale().x,
             anim->GetSpriteH() * anim->sprite.getScale().y };

    //position will always be top left
    bbox.pos = { pos.x - anim->GetOrigin().x * size.x,
                 pos.y - anim->GetOrigin().y * size.y };
    bbox.size = size;
}

void Entity::Draw() {
    anim->Draw();
}

void Entity::PlaySound() {
    //Play assigned noise with slight pitch shift
    float pitch_shift = (rand() % 10) * .01;
    if (rand() % 2) pitch_shift *= -1;
    sound.setPitch(sound.getPitch() + pitch_shift);
    sound.play();
    sound.setPitch(1);
}