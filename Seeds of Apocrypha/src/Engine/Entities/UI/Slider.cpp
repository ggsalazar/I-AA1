#include "Slider.h"

Slider::Slider(const Engine& e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style,
    const uint init_ui_layer, const int init_dfc) :
    UI(e, m, a_i, t, style, init_ui_layer, init_dfc),
    knob_val(engine.game.default_font), knob_tex("assets/Sprites/UI/SliderKnob.png"), knob_spr(knob_tex) {

    label_offset = engine.game.GetResScale()*16;
    label.setPosition(Vector2f(pos.x, pos.y - label_offset));

    //Set the origin/anchor, scale, and position
    knob_spr.setOrigin({ t.origin.x * 6.f, t.origin.y * 18.f }); //The knob is 6px wide & 18px high
    knob_spr.setScale(Vector2f(t.scale));
    knob_pos_max = bbox.position.x + bbox.size.x * .9f;
    knob_pos_min = bbox.position.x + bbox.size.x * .1f;
    //Setting knob position based on appropriate value
    string rounded_val = "";
    if (elem == UIElems::MUSIC_V or elem == UIElems::SFX_V) {
        float vol = elem == UIElems::MUSIC_V ? engine.game.GetMusicVolume() : engine.game.GetSFXVolume();
        knob_pos = (((knob_pos_max-knob_pos_min)*vol)*.01) + knob_pos_min;

        //Set the value
        rounded_val = to_string((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100);
        rounded_val = rounded_val.substr(0, rounded_val.find('.') + 3);
    }
    else if (elem == UIElems::CAMSPD) {
        knob_pos = ((((2*engine.game.cam_move_spd)-10)*.05) * (knob_pos_max - knob_pos_min)) + knob_pos_min;

        //Set the value
        knob_val = label;
        rounded_val = to_string((round((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 20) + 10) * .5);
        rounded_val = rounded_val.substr(0, rounded_val.find('.') + 2);
    }
    knob_spr.setPosition(Vector2f(knob_pos, pos.y));

    knob_val = label;
    Text::SetStr(knob_val, rounded_val);
    knob_val.setPosition(Vector2f(pos.x, pos.y + label_offset));
    
}

void Slider::GetInput() {
    UI::GetInput();

    if (active and Selected(MOUSEPOS_W_E) and BUTTONDOWN(LMB)) {
        
        //Adjust knob position
        knob_pos = MOUSEPOS_W_E.x;
        Math::Clamp(knob_pos, knob_pos_min, knob_pos_max);
        knob_spr.setPosition(Vector2f(knob_pos, pos.y));

        float new_val = 0;
        uint dec_place = 0;
        if (elem == UIElems::MUSIC_V or elem == UIElems::SFX_V) {
            new_val = (knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100;

            if (elem == UIElems::MUSIC_V)
                engine.game.SetMusicVolume(new_val);
            else if (elem == UIElems::SFX_V)
                engine.game.SetSFXVolume(new_val);

            dec_place = 3;
        }
        else if (elem == UIElems::CAMSPD) {
            //Camera speed goes from 5 to 15 and increments by .5
            new_val = (round((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 20) + 10) * .5;
            engine.game.cam_move_spd = new_val;

            dec_place = 2;
        }

        //Set the string
        string r_val = to_string(new_val);
        r_val = r_val.substr(0, r_val.find('.') + dec_place);
        Text::SetStr(knob_val, r_val);
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
    label.setPosition(Vector2f(pos.x, pos.y - label_offset));

    //Resize and move the knob
    knob_spr.setOrigin({ anim->GetOrigin().x * 6, anim->GetOrigin().y * 18 });
    knob_spr.setScale(anim->sprite.getScale());
    knob_pos_max = bbox.position.x + bbox.size.x * .9f;
    knob_pos_min = bbox.position.x + bbox.size.x * .1f;

    if (elem == UIElems::MUSIC_V)
        knob_pos = knob_pos_min + (engine.game.GetMusicVolume() * .01 * (knob_pos_max - knob_pos_min));
    else if (elem == UIElems::SFX_V)
        knob_pos = knob_pos_min + (engine.game.GetSFXVolume() * .01 * (knob_pos_max - knob_pos_min));
    else if (elem == UIElems::CAMSPD)
        knob_pos = ((((2 * engine.game.cam_move_spd) - 10) * .05) * (knob_pos_max - knob_pos_min)) + knob_pos_min;

    knob_spr.setPosition(Vector2f(knob_pos, pos.y));

    //Set the value
    knob_val.setPosition(Vector2f(pos.x, pos.y + label_offset));

}
