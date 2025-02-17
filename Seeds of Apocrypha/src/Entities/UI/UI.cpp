#include "UI.h"

UI::UI(const Structure& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style, const int init_dfc) :
	Entity(s, a_i, t, init_dfc), menu(m), elem(style.elem), label(structure.game.default_font) {
    
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

        case UIElems::CHARCREA:
            l_str = "New Adventure";
        break;

        case UIElems::CLASS:
            l_str = "Class";
        break;

        case UIElems::CREATE:
            l_str = "Create";
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

        case UIElems::MAINMENU:
            l_str = "Main Menu";
        break;

        case UIElems::MUSIC_V:
            l_str = "Music Volume";
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

        case UIElems::TUTORIAL:
            l_str = "Tutorial";
        break;
    }
    Text::Init(label, structure.game.default_font, style.font_size, pos, l_str);

    //Sound
    {
        if (!sb.loadFromFile("assets/SFX/ButtonClick.mp3")) {
            cerr << "Failed to load sound for UI element!" << endl;
            return;
        }
        sound.setBuffer(sb);
        sound.setVolume(structure.game.GetSFXVolume());
    }
}

void UI::GetInput() {
    if (active and Selected(MOUSEPOS_S)) {
        if (Input::KeyPressed(LMB))
            Pressed();

        if (Input::KeyReleased(LMB) and primed)
            Released();
    }
    else if (!Selected(MOUSEPOS_S)) primed = false;
}

void UI::Resize(uint r_s) {
    //Get the old origin
    sf::Vector2f old_origin = anim->GetOrigin();

    //Set sprite w/h
    anim->sprite.setScale({ r_s*1.f, r_s*1.f });
    //Set new sprite origin
    anim->SetOrigin(old_origin);
    //Adjust label size
    label.setCharacterSize(r_s * 16);
    //Reset the origin
    Text::SetOrigin(label);
    //Label offset and position are handled in children
}

void UI::SetActive(const bool new_active) {
    active = new_active;
    sf::Color color = anim->sprite.getColor();
    color.a = active ? 255 : 125;
    anim->sprite.setColor(color);
}

void UI::Pressed() {
    primed = true;
    sound.play();
}
