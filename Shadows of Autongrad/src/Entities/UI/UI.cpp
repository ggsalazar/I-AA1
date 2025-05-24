#include "UI.h"

UI::UI(Menu& m, const Sprite::Info& s_i, const UIElem e, const uint init_ui_layer)
    : Entity(s_i), menu(m), elem(e), ui_layer(init_ui_layer), label(&game->default_fonts[36]) {


    //Label
    string l_str = "DEFAULT";
    switch (elem) {
    case UIElem::Apply:
        l_str = "Apply";
        SetActive(false);
        break;

    case UIElem::AS:
        l_str = "Ability Scores";
        break;

    case UIElem::ASSTR:
        l_str = "Strength";
        break;

    case UIElem::ASCON:
        l_str = "Constitution";
        break;

    case UIElem::ASAGI:
        l_str = "Agility";
        break;

    case UIElem::ASDEX:
        l_str = "Dexterity";
        break;

    case UIElem::ASINT:
        l_str = "Intelligence";
        break;

    case UIElem::ASWIS:
        l_str = "Wisdom";
        break;

    case UIElem::ASCHA:
        l_str = "Charisma";
        break;

    case UIElem::Back:
        l_str = "Back";
        break;

    case UIElem::BG_B:
        l_str = "Background";
        break;

    case UIElem::CAMSPD:
        l_str = "Camera Speed";
        break;

    case UIElem::CharCrea:
        l_str = "New Adventure";
        break;

    case UIElem::Class_B:
        l_str = "Class";
        break;

    case UIElem::Class_P:
        l_str = "Class";
        break;

    case UIElem::Close:
        l_str = "Close";
        break;

    case UIElem::Create:
        l_str = "Create";
        break;

    case UIElem::Debug_Room:
        l_str = "Debug Room";
        break;

    case UIElem::Edge_Pan:
        l_str = "Edge Panning";
        break;

    case UIElem::Equipment_CC:
        l_str = "Equipment";
        break;

    case UIElem::Fullscreen:
        l_str = "Fullscreen";
        break;

    case UIElem::Load:
        l_str = "Continue Adventure";
        //Check to see if an existing save files exists before setting (in)active - TO-DO
        SetActive(false);
        break;

    case UIElem::Music_V:
        l_str = "Music Volume";
        break;

    case UIElem::No:
        l_str = "No";
        break;

    case UIElem::Options:
        l_str = "Options";
        break;

    case UIElem::Quit:
        l_str = "Quit";
        break;

    case UIElem::Race_B:
        l_str = "Race";
        break;

    case UIElem::Race_P:
        l_str = "Racial Appearance";
        break;

    case UIElem::Resolution:
        l_str = "Resolution";
        break;

    case UIElem::Resume:
        l_str = "Resume";
        break;

    case UIElem::Sex:
        l_str = "Sex";
        break;

    case UIElem::SFX_V:
        l_str = "SFX Volume";
        break;

    case UIElem::Size:
        l_str = "Size";
        break;

    case UIElem::Skills:
        l_str = "Skills";
        break;

    case UIElem::Title:
        l_str = "Return to Title";
        break;

    case UIElem::Tutorial:
        l_str = "Tutorial";
        break;

    case UIElem::Yes:
        l_str = "Yes";
        break;
    }

    label.info.pos = pos;
    label.info.str = l_str;
    label.SetOrigin();

    //Sound
    /*
    if (!sb.loadFromFile("assets/SFX/ButtonClick.mp3")) {
        cerr << "Failed to load sound for UI element!" << endl;
        return;
    }
    sound.setBuffer(sb);
    sound.setVolume(game->GetSFXVolume());
    */

}

void UI::GetInput() {
    if (Selected()) { //Selected() accounts for current state of active
        if (Input::BtnPressed(LMB))
            Pressed();

        if (Input::BtnReleased(LMB) and primed)
            Released();
    }
    else if (!Selected()) primed = false;
}

void UI::Draw() {
}

void UI::Resize(uint r_s) {
    //Set sprite w/h
    sprite.SetScale(Vector2i{ (int)r_s });
    //Label offset and position are handled in children
}

void UI::SetActive(const bool new_active) {
    active = new_active;
    Color new_color = sprite.GetColor();
    new_color.a = active ? 1.f : .5f;
    sprite.SetColor(new_color);

    if (!active)
        primed = false;

}

void UI::Pressed() {
    primed = true;
    //sound.play();
}
