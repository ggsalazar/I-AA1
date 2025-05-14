#include "UI.h"

UI::UI(Game& g, Scene* s, Menu& m, const Sprite::Info& s_i, const UIElems e, const uint init_ui_layer)
	: Entity(g, s, s_i), menu(m), elem(e), ui_layer(init_ui_layer), label(make_u<Text>(game.default_font36.get())) {
    
    //Label
    string l_str = "DEFAULT";
    switch (elem) {
        case UIElems::APPLY:
            l_str = "Apply";
            SetActive(false);
        break;

        case UIElems::AS:
            l_str = "Ability Scores";
        break;

        case UIElems::ASSTR:
            l_str = "Strength";
        break;

        case UIElems::ASCON:
            l_str = "Constitution";
        break;

        case UIElems::ASAGI:
            l_str = "Agility";
        break;

        case UIElems::ASDEX:
            l_str = "Dexterity";
        break;

        case UIElems::ASINT:
            l_str = "Intelligence";
        break;

        case UIElems::ASWIS:
            l_str = "Wisdom";
        break;

        case UIElems::ASCHA:
            l_str = "Charisma";
        break;

        case UIElems::BACK:
            l_str = "Back";
        break;

        case UIElems::BACKGROUND_B:
            l_str = "Background";
        break;

        case UIElems::CAMSPD:
            l_str = "Camera Speed";
        break;

        case UIElems::CHARCREA:
            l_str = "New Adventure";
        break;

        case UIElems::CLASS_B:
            l_str = "Class";
        break;

        case UIElems::CLASS_P:
            l_str = "Class";
        break;

        case UIElems::CLOSE:
            l_str = "Close";
        break;

        case UIElems::CREATE:
            l_str = "Create";
        break;

        case UIElems::DEBUG_ROOM:
            l_str = "Debug Room";
        break;

        case UIElems::EQUIPMENT_CC:
            l_str = "Equipment";
        break;

        case UIElems::FULLSCREEN:
            l_str = "Fullscreen";
        break;

        case UIElems::LOAD:
            l_str = "Continue Adventure";
            //Check to see if an existing save files exists before setting (in)active - TO-DO
            SetActive(false);
        break;

        case UIElems::MUSIC_V:
            l_str = "Music Volume";
        break;

        case UIElems::NO:
            l_str = "No";
        break;

        case UIElems::OPTIONS:
            l_str = "Options";
        break;

        case UIElems::QUIT:
            l_str = "Quit";
        break;

        case UIElems::RACE_B:
            l_str = "Race";
        break;

        case UIElems::RACE_P:
            l_str = "Racial Appearance";
        break;

        case UIElems::RESOLUTION:
            l_str = "Resolution";
        break;

        case UIElems::RESUME:
            l_str = "Resume";
        break;

        case UIElems::SEX:
            l_str = "Sex";
        break;

        case UIElems::SFX_V:
            l_str = "SFX Volume";
        break;

        case UIElems::SIZE:
            l_str = "Size";
        break;

        case UIElems::SKILLS:
            l_str = "Skills";
        break;

        case UIElems::TITLE:
            l_str = "Return to Title";
        break;

        case UIElems::TUTORIAL:
            l_str = "Tutorial";
        break;

        case UIElems::YES:
            l_str = "Yes";
        break;
    }

    label->info.pos = pos;
    label->info.str = l_str;
    label->SetOrigin();

    //Sound
    /*
    if (!sb.loadFromFile("assets/SFX/ButtonClick.mp3")) {
        cerr << "Failed to load sound for UI element!" << endl;
        return;
    }
    sound.setBuffer(sb);
    sound.setVolume(engine.game.GetSFXVolume());
    */

}

void UI::GetInput() {
    if (active and Selected()) {
        if (Input::BtnPressed(LMB))
            Pressed();

        if (Input::BtnReleased(LMB) and primed)
            Released();
    }
    else if (!Selected()) primed = false;
}

void UI::Resize(uint r_s) {
    //Set sprite w/h
    sprite->SetScale(Vector2i{ (int)r_s });
    //Adjust label size
    //label->info.char_size = r_s * 14;
    //Label offset and position are handled in children
}

void UI::SetActive(const bool new_active) {
    active = new_active;
    Color new_color = sprite->GetColor();
    new_color.a = active ? 1.f : .5f;
    sprite->SetColor(new_color);
}

void UI::Pressed() {
    primed = true;
    //sound.play();
}
