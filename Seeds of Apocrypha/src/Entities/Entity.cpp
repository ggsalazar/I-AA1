#include "Entity.h"

Entity::Entity(const Engine& e, const AnimInfo& a_i, const Animation::Transform& t, int init_dfc) :
    engine(e), pos(t.pos),
    dfc(init_dfc), sound(sb), anim(make_unique<Animation>(engine.game, engine.window, this, a_i, t)) {

    SetBBox();
    bbox_debug.setFillColor(sf::Color(0, 255, 0, 127)); //Green, 50% opacity
    pos_debug.setSize({ 4, 4 });
    pos_debug.setFillColor(sf::Color(255, 0, 0, 127)); //Red, 50% opacity
}

void Entity::Move(sf::Vector2f offset) {
    pos += offset;
    anim->sprite.setPosition(pos);
    SetBBox();
}

void Entity::MoveTo(sf::Vector2f new_pos) {
    pos = new_pos;
    anim->sprite.setPosition(pos);
    SetBBox();
}

void Entity::SetBBox() {
    pos_debug.setPosition(sf::Vector2f(pos.x - 2, pos.y - 2));

    w = anim->GetSpriteW() * anim->sprite.getScale().x;
    h = anim->GetSpriteH() * anim->sprite.getScale().y;

    //position.x/y will always be left+top
    bbox.position.x = pos.x - anim->GetOrigin().x * w;
    bbox.position.y = pos.y - anim->GetOrigin().y * h;
    bbox.size.x = w;
    bbox.size.y = h;

    //bbox debug
    bbox_debug.setPosition(bbox.position);
    bbox_debug.setSize(bbox.size);
}

void Entity::Draw() {
    anim->Draw();

    if (engine.game.debug) {
        engine.window.draw(bbox_debug);
        engine.window.draw(pos_debug);
    }
}

void Entity::PlaySound() {
    //Play assigned noise with slight pitch shift
    float pitch_shift = (rand() % 10) * .01;
    if (rand() % 2) pitch_shift *= -1;
    sound.setPitch(sound.getPitch() + pitch_shift);
    sound.play();
    sound.setPitch(1);
}
