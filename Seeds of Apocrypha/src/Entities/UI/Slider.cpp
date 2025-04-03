#include "Slider.h"

Slider::Slider(const Engine& e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style,
    const uint init_ui_layer, const int init_dfc) :
    UI(e, m, a_i, t, style, init_ui_layer, init_dfc),
    knob_val(engine.game.default_font), knob_tex("assets/Sprites/UI/SliderKnob.png"), knob_spr(knob_tex) {

    label_offset = engine.game.GetResScale()*10;
    label.setPosition(sf::Vector2f(pos.x, pos.y - label_offset));

    //Set the origin/anchor, scale, and position
    knob_spr.setOrigin({ t.origin.x * 6.f, t.origin.y * 18.f }); //The knob is 6px wide & 18px high
    knob_spr.setScale(sf::Vector2f(t.scale));
    knob_pos_max = bbox.position.x + bbox.size.x * .9f;
    knob_pos_min = bbox.position.x + bbox.size.x * .1f;
    knob_pos = knob_pos_max;
    knob_spr.setPosition(sf::Vector2f(knob_pos, pos.y));

    //Set the value
    knob_val = label;
    string rounded_val = to_string((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100);
    rounded_val = rounded_val.substr(0, rounded_val.find('.') + 3);
    knob_val.setString(rounded_val);
    Text::SetOrigin(knob_val);
    knob_val.setPosition(sf::Vector2f(pos.x, pos.y + label_offset));
}

void Slider::GetInput() {
    UI::GetInput();

    if (active and Selected(MOUSEPOS_E) and BUTTONDOWN(LMB)) {
        
        //Adjust knob position
        knob_pos = MOUSEPOS_E.x;
        Math::Clamp(knob_pos, knob_pos_min, knob_pos_max);
        knob_spr.setPosition(sf::Vector2f(knob_pos, pos.y));

        float new_vol = (knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100;
        if (elem == UIElems::MUSIC_V)
            engine.game.SetMusicVolume(new_vol);
        else if (elem == UIElems::SFX_V)
            engine.game.SetSFXVolume(new_vol);

        string r_val = to_string(new_vol);
        r_val = r_val.substr(0, r_val.find('.') + 3);
        knob_val.setString(r_val);
    }
}

void Slider::Draw() {
    Entity::Draw();
    engine.window.draw(label);
    engine.window.draw(knob_val);
    engine.window.draw(knob_spr);
}

void Slider::Move() {
    label_offset = engine.game.GetResScale() * 6;
    label.setPosition(sf::Vector2f(pos.x, pos.y - label_offset));

    //Resize and move the knob
    knob_spr.setOrigin({ anim->GetOrigin().x * 6, anim->GetOrigin().y * 18 });
    knob_spr.setScale(anim->sprite.getScale());
    knob_pos_max = bbox.position.x + bbox.size.x * .9f;
    knob_pos_min = bbox.position.x + bbox.size.x * .1f;
    if (elem == UIElems::MUSIC_V)
        knob_pos = knob_pos_min + (engine.game.GetMusicVolume() * .01 * (knob_pos_max - knob_pos_min));
    else if (elem == UIElems::SFX_V)
        knob_pos = knob_pos_min + (engine.game.GetSFXVolume() * .01 * (knob_pos_max - knob_pos_min));
    knob_spr.setPosition(sf::Vector2f(knob_pos, pos.y));
    //Set the value
    knob_val = label;
    string rounded_val = to_string((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100);
    rounded_val = rounded_val.substr(0, rounded_val.find('.') + 3);
    knob_val.setString(rounded_val);
    Text::SetOrigin(knob_val);
    knob_val.setPosition(sf::Vector2f(pos.x, pos.y + label_offset));
}
