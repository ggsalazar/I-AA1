#include "Slider.h"
#include "../../Core/Math.h"

Slider::Slider(Menu& m, const Sprite::Info& s_i, const UIElem e,
    const uint init_ui_layer)
    : UI(m, s_i, e, init_ui_layer),
    knob_label(&game->default_fonts[36]) {

    

    Sprite::Info knob_info = {}; knob_info.sheet = "UI/SliderKnob"; knob_info.frame_size = { 6, 18 };
    knob_info.scale = sprite.GetScale();
    knob_spr.Init(knob_info);

    label_offset = game->GetResScale() * 6;
    label.MoveTo({ pos.x, pos.y - label_offset });
    label.SetOrigin();

    //Set the origin/anchor, scale, and position
    knob_spr.SetOrigin();
    knob_pos_max = bbox.x + bbox.w * .9f;
    knob_pos_min = bbox.x + bbox.w * .1f;

    //Setting knob position based on appropriate value
    string rounded_val = "";
    if (elem == UIElem::Music_V or elem == UIElem::SFX_V) {
        float vol = elem == UIElem::Music_V ? game->GetMusicVolume() : game->GetSFXVolume();
        knob_pos = (((knob_pos_max - knob_pos_min) * vol) * .01) + knob_pos_min;

        //Set the value
        rounded_val = to_string((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100);
        rounded_val = rounded_val.substr(0, rounded_val.find('.') + 3);
    }
    else if (elem == UIElem::CAMSPD) {
        knob_pos = ((((2 * game->cam_move_spd)-2) /18) * (knob_pos_max - knob_pos_min)) + knob_pos_min;

        //Set the value
        rounded_val = to_string((round((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 18) + 2) * .5);
        rounded_val = rounded_val.substr(0, rounded_val.find('.') + 2);
    }
    knob_spr.MoveTo({ knob_pos, pos.y });

    knob_label.SetOrigin();
    knob_label.info.str = rounded_val;
    knob_label.MoveTo({ pos.x, pos.y + label_offset });
}

void Slider::GetInput() {
    UI::GetInput();

    if (Selected() and Input::BtnDown(LMB)) {

        //Adjust knob position
        knob_pos = Input::MousePos().x;
        Math::Clamp(knob_pos, knob_pos_min, knob_pos_max);
        knob_spr.MoveTo({ knob_pos, pos.y });

        float new_val = 0;
        uint dec_place = 0;
        if (elem == UIElem::Music_V or elem == UIElem::SFX_V) {
            new_val = (knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 100;

            if (elem == UIElem::Music_V)
                game->SetMusicVolume(new_val);
            else if (elem == UIElem::SFX_V)
                game->SetSFXVolume(new_val);

            dec_place = 3;
        }
        else if (elem == UIElem::CAMSPD) {
            //Camera speed goes from 1 to 10 in increments of .5
            new_val = (round((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 18) + 2) * .5;
            game->cam_move_spd = new_val;

            dec_place = 2;
        }

        //Set the string
        string r_val = to_string(new_val);
        knob_label.info.str = r_val.substr(0, r_val.find('.') + dec_place);
    }
}

void Slider::Draw() {
    Entity::Draw();

    game->renderer.DrawTxt(label);
    game->renderer.DrawTxt(knob_label);
    game->renderer.DrawSprite(knob_spr);
}

void Slider::Move() {
    //Move debug and bbox
    pos_debug.x = pos.x;
    pos_debug.y = pos.y;

    size = sprite.GetSprSize();

    //bbox position will always be top left
    bbox.x = pos.x - sprite.GetOrigin().x * size.x;
    bbox.y = pos.y - sprite.GetOrigin().y * size.y;
    bbox.w = size.x;
    bbox.h = size.y;

    //Move everything else
    label_offset = game->GetResScale() * 6;
    label.MoveTo({ pos.x, pos.y - label_offset });

    //Resize and move the knob
    knob_spr.SetScale(sprite.GetScale());
    knob_pos_max = bbox.x + bbox.w * .9f;
    knob_pos_min = bbox.x + bbox.w * .1f;

    if (elem == UIElem::Music_V)
        knob_pos = knob_pos_min + (game->GetMusicVolume() * .01 * (knob_pos_max - knob_pos_min));
    else if (elem == UIElem::SFX_V)
        knob_pos = knob_pos_min + (game->GetSFXVolume() * .01 * (knob_pos_max - knob_pos_min));
    else if (elem == UIElem::CAMSPD)
        knob_pos = ((round((2 * game->cam_move_spd)-2) /18) * (knob_pos_max - knob_pos_min)) + knob_pos_min;

    knob_spr.MoveTo({ knob_pos, pos.y });

    //Set the value
    knob_label.MoveTo({ pos.x, pos.y + label_offset });
}
