#include "Slider.h"

Slider::Slider(const Structure& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style, const int init_dfc) :
    UI(s, m, a_i, t, style, init_dfc), knob_val(structure.game.default_font), knob_spr(knob_tex) {

    label_offset = structure.game.GetResScale()*6;
    label.setPosition({ pos.x, pos.y - label_offset });

    //Img/Tex/Spr for the knob
    if (!knob_img.loadFromFile("assets/Sprites/UI/SliderKnob.png")) {
        std::cerr << "Failed to load image for SliderKnob!" << std::endl;
        return;
    }
    if (!knob_tex.loadFromImage(knob_img)) {
        std::cerr << "Failed to load texture for SliderKnob!" << std::endl;
        return;
    }
    knob_spr.setTexture(knob_tex);

    //Set the origin/anchor, scale, and position
    knob_spr.setOrigin({ t.origin.x * 3.f, t.origin.y * 9.f }); //The knob is 3px wide & 9px high
    knob_spr.setScale(t.scale);
    knob_pos_max = bbox.position.x + bbox.size.x * .9f;
    knob_pos_min = bbox.position.x + bbox.size.x * .1f;
    knob_pos = knob_pos_max;
    knob_spr.setPosition({ knob_pos, pos.y });

    //Set the value
    knob_val = label;
    string rounded_val = to_string((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100);
    rounded_val = rounded_val.substr(0, rounded_val.find('.') + 3);
    knob_val.setString(rounded_val);
    Text::SetOrigin(knob_val);
    knob_val.setPosition({ pos.x, pos.y + label_offset });
}

void Slider::GetInput() {
    UI::GetInput();

    if (active and Selected(MOUSEPOS) and LMBDOWN()) {
        
        //Adjust knob position
        knob_pos = MOUSEPOS.x;
        Util::Clamp(knob_pos, knob_pos_min, knob_pos_max);
        knob_spr.setPosition({ knob_pos, pos.y });

        float new_vol = (knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100;
        if (elem == UIElems::MUSIC_V)
            structure.game.SetMusicVolume(new_vol);
        else if (elem == UIElems::SFX_V)
            structure.game.SetSFXVolume(new_vol);

        string r_val = to_string(new_vol);
        r_val = r_val.substr(0, r_val.find('.') + 3);
        knob_val.setString(r_val);
    }
}

void Slider::Draw(const bool debug) {
    Entity::Draw(debug);
    structure.window.draw(label);
    structure.window.draw(knob_val);
    structure.window.draw(knob_spr);
}

void Slider::Move() {
    Entity::Move();
    label_offset = structure.game.GetResScale() * 6;
    label.setPosition({ pos.x, pos.y - label_offset });

    //Resize and move the knob
    knob_spr.setOrigin({ anim->GetOrigin().x * 3, anim->GetOrigin().y * 9 });
    knob_spr.setScale(anim->sprite.getScale());
    knob_pos_max = bbox.position.x + bbox.size.x * .9f;
    knob_pos_min = bbox.position.x + bbox.size.x * .1f;
    //Where to move it to????
    if (elem == UIElems::MUSIC_V)
        knob_pos = knob_pos_min + (structure.game.GetMusicVolume() * .01 * (knob_pos_max - knob_pos_min));
    else if (elem == UIElems::SFX_V)
        knob_pos = knob_pos_min + (structure.game.GetSFXVolume() * .01 * (knob_pos_max - knob_pos_min));
    knob_spr.setPosition({ knob_pos, pos.y });
    //Set the value
    knob_val = label;
    string rounded_val = to_string((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100);
    rounded_val = rounded_val.substr(0, rounded_val.find('.') + 3);
    knob_val.setString(rounded_val);
    Text::SetOrigin(knob_val);
    knob_val.setPosition({ pos.x, pos.y + label_offset });
}