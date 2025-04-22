#include "../Game.h"
#include "../../../Engine/Entities/Entity.h"

#ifdef PlaySound
#undef PlaySound
#endif

//Needs to be part of the Engine namespace since it is the implementation of the base class, which is part of the Engine namespace
namespace Engine {

Entity::Entity(Game& g, Scene* s, const string& sheet, const Sprite::Info& s_i) :
    game(g), scene(s), pos(s_i.pos), pos_debug(pos, 2),
    /*sound(sb),*/ sprite(make_unique<Sprite_D2D>(sheet, game.renderer->GetRT(), s_i)) {
    SetBBox();
}

void Entity::SetBBox() {
    pos_debug.pos = pos;

    size = sprite->GetSprSize();

    //bbox position will always be top left
    bbox.pos = { (int)(pos.x - sprite->GetOrigin().x * size.x),
                    (int)(pos.y - sprite->GetOrigin().y * size.y) };
    bbox.size = size;
}

void Entity::Draw() {
    game.renderer->DrawSprite(*sprite);
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
}