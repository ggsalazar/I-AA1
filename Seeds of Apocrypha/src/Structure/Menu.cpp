#include "Menu.h"
#include "Game.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(Game& g, sf::RenderWindow& w, Scene& s, Menus init_label) :
            game(g), window(w), scene(s), label(init_label), menu_text(game.default_font), sup_text(game.default_font) {
    
    float res_scalar = game.GetResScale();
    int style_size = res_scalar * 12;
    ui_size = { res_scalar, res_scalar };

    Text::Init(menu_text, game.default_font, res_scalar * 36, { window.getSize().x * .5f, window.getSize().y * .3f }, "DEFAULT");
    Text::Init(sup_text, game.default_font, res_scalar * 24, { window.getSize().x * .5f, window.getSize().y * .4f }, "SUPPLEMENTARY DEFAULT");

    //What we do depends on our label
    switch (label) {
        //Menus
        case Menus::CHARCREA: {
            /*Things to determine:
            Race/Racial features (aesthetics + mechanics),
            Size,
            Sex,
            Class,
            Background,
            Skills,
            Ability scores,
            Starting equipment,
            Name
            */
            //Menu text
            Text::SetCharSize(menu_text, res_scalar * 24);
            menu_text.setPosition({ window.getSize().x * .5f, window.getSize().y * .1f });
            Text::SetStr(menu_text, "Create Your Party");
            //Supp. text
            Text::SetCharSize(sup_text, res_scalar * 18);
            Text::SetOrigin(sup_text, { .5f, .0f });
            sup_text.setPosition({ window.getSize().x * .5f, window.getSize().y * .13f });
            Text::SetStr(sup_text, "Use the options below to create your party of 4 adventurers");
            
            sf::Vector2f picker_pos = { window.getSize().x * .2f, window.getSize().y * .2f };
            float p_y_buffer = window.getSize().y*.12f;

            auto race_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ picker_pos, ui_ori, ui_size },
                UI::Style{ UIElems::RACE, style_size });
            auto race_sm = make_unique<Menu>(game, window, scene, Menus::CCRACE);

            picker_pos.y += p_y_buffer;
            auto bg_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ picker_pos, ui_ori, ui_size },
                UI::Style{ UIElems::BACKGROUND, style_size });
            auto bg_sm = make_unique<Menu>(game, window, scene, Menus::CCBG);

            picker_pos.y += p_y_buffer;
            auto class_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ picker_pos, ui_ori, ui_size },
                UI::Style{ UIElems::CLASS, style_size });
            auto class_sm = make_unique<Menu>(game, window, scene, Menus::CCCLASS);

            picker_pos.y += p_y_buffer;
            auto ab_sc_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ picker_pos, ui_ori, ui_size },
                UI::Style{ UIElems::AS, style_size });
            auto ab_sc_sm = make_unique<Menu>(game, window, scene, Menus::CCAS);

            picker_pos.y += p_y_buffer;
            auto skills_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ picker_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SKILLS, style_size });
            auto skills_sm = make_unique<Menu>(game, window, scene, Menus::CCSKILLS);

            picker_pos.y += p_y_buffer;
            auto equip_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ picker_pos, ui_ori, ui_size },
                UI::Style{ UIElems::EQUIPMENTCC, style_size });
            auto equip_sm = make_unique<Menu>(game, window, scene, Menus::CCEQUIP);
            //THEN: RACE MECHANICS, RACE AESTHETICS, WARRIOR FIGHTING-STYLE SELECTION, ROGUE EXPERTISE SELECTION, ARCANIST SPELL SELECTION


            auto create_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ {window.getSize().x * .5f, window.getSize().y * .8f}, ui_ori, ui_size },
                UI::Style{ UIElems::CREATE, style_size });
            auto back_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ {window.getSize().x * .5f, window.getSize().y * .9f}, ui_ori, ui_size },
                UI::Style{ UIElems::BACK, style_size });

            ui_elems.insert(make_pair(UIElems::RACE, race_picker));
            ui_elems.insert(make_pair(UIElems::BACKGROUND, bg_picker));
            ui_elems.insert(make_pair(UIElems::CLASS, class_picker));
            ui_elems.insert(make_pair(UIElems::AS, ab_sc_btn));
            ui_elems.insert(make_pair(UIElems::SKILLS, skills_btn));
            ui_elems.insert(make_pair(UIElems::EQUIPMENTCC, equip_btn));

            ui_elems.insert(make_pair(UIElems::CREATE, create_btn));
            ui_elems.insert(make_pair(UIElems::BACK, back_btn));
            
            sub_menus.insert(make_pair(Menus::CCRACE, move(race_sm)));
            sub_menus.insert(make_pair(Menus::CCBG, move(bg_sm)));
            sub_menus.insert(make_pair(Menus::CCCLASS, move(class_sm)));
            sub_menus.insert(make_pair(Menus::CCAS, move(ab_sc_sm)));
            sub_menus.insert(make_pair(Menus::CCSKILLS, move(skills_sm)));
            sub_menus.insert(make_pair(Menus::CCEQUIP, move(equip_sm)));
            break;
        }

        //Character Creation Sub-Menus
        case Menus::CCAS: {
            //Ability Scores: STR, CON, AGI, DEX, INT, WIS, CHA
            //Picker for each
            break;
        }

        case Menus::CCBG: {
            //Backgrounds: Custom, Artist, Divine, Farmer, Innkeeper, Mechanic, Sailor, Soldier, Tradesman
            //Backgrounds give an ASI, Skills, and some equipment
            break;
        }

        case Menus::CCCLASS: {
            //Classes: Arcanist, Rogue, Warrior
            //Health / Skills / L1 features:
            //Arcanist: 6 / Arcana / Spells
            //Rogue: 8 / Thieving Tools / Expertise
            //Warrior: 10 / 2 PWS / Fighting Style
            break;
        }

        case Menus::CCEQUIP: {
            //Characters get 10 Aeons and 50 change, one weapon that they are proficient in, 3 rations, and a 
            // (plot-relevant?) token from their background
            break;
        }

        case Menus::CCRACE: {
            //Races: Automaton (locks mechanical features & sex), Dwarf, Elf, Gnome, Human, Kobold
            //Race largely determines player aesthetics and influences several mechanics, including:
            // Size, Ability Score Adjustments, Sex, Skills, etc.
            //Elves also have a sub-race that limits their aesthetics
            //Aesthetics have suggested features, but none are locked out from any aesthetics (except Automata)
            //Menu text
            Text::SetCharSize(menu_text, res_scalar * 16);
            menu_text.setPosition({ window.getSize().x * .5f, window.getSize().y * .2f });
            Text::SetStr(menu_text, "Race");
            //Supp. text
            Text::SetCharSize(sup_text, res_scalar * 12);
            Text::SetOrigin(sup_text, { .5f, .0f });
            sup_text.setPosition({ window.getSize().x * .5f, window.getSize().y * .23f });
            Text::SetStr(sup_text, "Your Race influences your appearance, some of your capabilities, and how others react to you.");

            sf::Vector2f picker_pos = { window.getSize().x * .5f, window.getSize().y * .5f };
            float p_y_buffer = window.getSize().y * .2f;

            auto size_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ picker_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SIZE, style_size });

            picker_pos.y += p_y_buffer;
            auto sex_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ picker_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SEX, style_size });

            break;
        }

        case Menus::CCSKILLS: {
            break;
        }
        //End of CC Sub-Menus

        case Menus::GO: {
            break;
        }

        case Menus::INVENTORY: {
            break;
        }

        case Menus::LEVELUP: {
            break;
        }

        case Menus::LOAD: {
            break;
        }

        case Menus::MAIN: {
            Text::SetStr(menu_text, "Seeds of Apocrypha");
            Text::SetStr(sup_text, "");

            sf::Vector2f btn_pos = { window.getSize().x * .5f, window.getSize().y * .5f };
            float b_y_buffer = window.getSize().y * .1f;

            auto char_crea_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::CHARCREA, style_size });
            btn_pos.y += b_y_buffer;
            auto load_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::LOAD, style_size });
            btn_pos.y += b_y_buffer;
            auto options_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::OPTIONS, style_size });
            btn_pos.y += b_y_buffer;
            auto quit_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::QUIT, style_size });

            ui_elems.insert(make_pair(UIElems::CHARCREA, char_crea_btn));
            ui_elems.insert(make_pair(UIElems::LOAD, load_btn));
            ui_elems.insert(make_pair(UIElems::OPTIONS, options_btn));
            ui_elems.insert(make_pair(UIElems::QUIT, quit_btn));
            break;
        }

        case Menus::OPTIONS: {
            break;
        }

        case Menus::PAUSE: {
            break;
        }
    }
    
    //Add our UI elements to the scene entities vector
    for (const auto& ui : ui_elems)
        scene.AddEntity(ui.second);
}

void Menu::Draw() {
    if (open) {
        window.draw(menu_text);
        window.draw(sup_text);
    }

    for (const auto& sm : sub_menus)
        sm.second->Draw();
}

void Menu::Open(const bool o) {
    open = o;
    if (!open) {
        for (const auto& sm : sub_menus)
            sm.second->Open(false);
    }
}

void Menu::Resize() {
    //Resize all text and UI elements according to the new res scalar and window dimensions
    float res_scalar = game.GetResScale();
    float w_x = window.getSize().x;
    float w_y = window.getSize().y;

    //A more efficient way to do this would be to base the new size on the old size and do some funky math shit with the scalar, but this works for now
    menu_text.setCharacterSize(res_scalar * 10);
    Text::SetOrigin(menu_text);
    menu_text.setPosition({ w_x * .5f, w_y * .2f });
    if (label == Menus::MAIN) {
        menu_text.setCharacterSize(res_scalar * 15);
        Text::SetOrigin(menu_text);
        menu_text.setPosition({ w_x * .5f, w_y * .1f });
    }

    //UI Elements
    for (const auto& ui : ui_elems) {
        auto& u = ui.second;

        u->Resize(res_scalar);

        //Adjust the positions
        switch (ui.first) {
            case UIElems::APPLY:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .8;
            break;

            case UIElems::BACK:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .9;
            break;

            case UIElems::FULLSCREEN:
                u->pos.x = w_x * (.5 + res_scalar * .02);
                u->pos.y = w_y * .675;
            break;

            case UIElems::MUSICV:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .3;
            break;

            case UIElems::OPTIONS:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .6;
            break;

            case UIElems::QUIT:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .7;
            break;

            case UIElems::RESOLUTION:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .55;
            break;

            case UIElems::SFXV:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .425;
            break;
        }
        u->SetBBox();
        u->Move();
    }
}

bool Menu::CheckUIElem(const UIElems ui) {
    if (ui_elems.count(ui) > 0) return true;
    else cout << "That UI Element does not exist!" << endl;
    return false;
}

void Menu::SetUIElemStatus(const UIElems ui, const string new_status) {
    if (CheckUIElem(ui)) {
        if (auto picker = dynamic_cast<Picker*>(ui_elems[ui].get()))
            picker->SetPicking(new_status);
    }
}

void Menu::SetUIElemActive(const UIElems ui, const bool a) {
    if (CheckUIElem(ui))
        ui_elems[ui]->SetActive(a);
}

string Menu::GetUIElemStatus(const UIElems ui) {
    if (CheckUIElem(ui)) {
        if (auto picker = dynamic_cast<Picker*>(ui_elems[ui].get()))
            return picker->GetPicking();
        else if (auto toggle = dynamic_cast<Toggle*>(ui_elems[ui].get())) {
            if (toggle->GetOn()) return "True";
            return "False";
        }
    }

    return "No such UIElem exists";
}

bool Menu::GetUIElemActive(const UIElems ui) {
    if (CheckUIElem(ui))
        return ui_elems[ui]->GetActive();

    return false;
}
