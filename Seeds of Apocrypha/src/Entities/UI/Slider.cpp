#include "Slider.h"

Slider::Slider(Game& g, Scene* s, Menu& m, const Sprite::Info& s_i, const UIElems e,
    const uint init_ui_layer)
    : UI(g, s, m, s_i, e, init_ui_layer),
      knob_label(make_u<Text>(game.default_font36.get())) {

    Sprite::Info knob_info = {}; knob_info.sheet = "assets/Sprites/UI/SliderKnob"; knob_info.frame_size = {6, 18};
    knob_info.scale = sprite->GetScale();
    knob_spr = make_u<Sprite>(game.renderer->GetRenderer(), knob_info);
    
    label_offset = game.GetResScale()*6;
    label->MoveTo(Vector2i(pos.x, pos.y - label_offset));
    label->SetOrigin();

    //Set the origin/anchor, scale, and position
    knob_spr->SetOrigin();
    knob_pos_max = bbox.x + bbox.w * .9f;
    knob_pos_min = bbox.x + bbox.w * .1f;

    //Setting knob position based on appropriate value
    string rounded_val = "";
    if (elem == UIElems::MUSIC_V or elem == UIElems::SFX_V) {
        float vol = elem == UIElems::MUSIC_V ? game.GetMusicVolume() : game.GetSFXVolume();
        knob_pos = (((knob_pos_max-knob_pos_min)*vol)*.01) + knob_pos_min;

        //Set the value
        rounded_val = to_string((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100);
        rounded_val = rounded_val.substr(0, rounded_val.find('.') + 3);
    }
    else if (elem == UIElems::CAMSPD) {
        knob_pos = ((((2*game.cam_move_spd)-10)*.05) * (knob_pos_max - knob_pos_min)) + knob_pos_min;

        //Set the value
        rounded_val = to_string((round((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 20) + 10) * .5);
        rounded_val = rounded_val.substr(0, rounded_val.find('.') + 2);
    }
    knob_spr->MoveTo({ knob_pos, pos.y });

    knob_label->SetOrigin();
    knob_label->info.str = rounded_val;
    knob_label->MoveTo({ pos.x, pos.y + label_offset });
}

void Slider::GetInput() {
    UI::GetInput();

    if (active and Selected() and Input::BtnDown(LMB)) {
        
        //Adjust knob position
        knob_pos = Input::MousePos().x;
        knob_pos = Math::Clamp(knob_pos, knob_pos_min, knob_pos_max);
        knob_spr->MoveTo({ knob_pos, pos.y });

        float new_val = 0;
        uint dec_place = 0;
        if (elem == UIElems::MUSIC_V or elem == UIElems::SFX_V) {
            new_val = (knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100;

            if (elem == UIElems::MUSIC_V)
                game.SetMusicVolume(new_val);
            else if (elem == UIElems::SFX_V)
                game.SetSFXVolume(new_val);

            dec_place = 3;
        }
        else if (elem == UIElems::CAMSPD) {
            //Camera speed goes from 5 to 15 and increments by .5
            new_val = (round((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 20) + 10) * .5;
            game.cam_move_spd = new_val;

            dec_place = 2;
        }

        //Set the string
        string r_val = to_string(new_val);
        knob_label->info.str = r_val.substr(0, r_val.find('.') + dec_place);
    }
}

void Slider::Draw() {
    Entity::Draw();

    game.renderer->DrawTxt(*label);
    game.renderer->DrawTxt(*knob_label);
    game.renderer->DrawSprite(*knob_spr);
}

void Slider::Move() {
    label_offset = game.GetResScale() * 6;
    label->MoveTo({ pos.x, pos.y - label_offset });

    //Resize and move the knob
    knob_spr->SetScale(sprite->GetScale());
    knob_pos_max = bbox.x + bbox.w * .9f;
    knob_pos_min = bbox.x + bbox.w * .1f;

    if (elem == UIElems::MUSIC_V)
        knob_pos = knob_pos_min + (game.GetMusicVolume() * .01 * (knob_pos_max - knob_pos_min));
    else if (elem == UIElems::SFX_V)
        knob_pos = knob_pos_min + (game.GetSFXVolume() * .01 * (knob_pos_max - knob_pos_min));
    else if (elem == UIElems::CAMSPD)
        knob_pos = ((((2 * game.cam_move_spd) - 10) * .05) * (knob_pos_max - knob_pos_min)) + knob_pos_min;

    knob_spr->MoveTo({ knob_pos, pos.y });

    //Set the value
    knob_label->MoveTo({ pos.x, pos.y + label_offset });
}
