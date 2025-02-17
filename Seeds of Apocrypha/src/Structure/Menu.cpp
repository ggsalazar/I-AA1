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

    Text::Init(menu_text, game.default_font, res_scalar * 36, { window.getSize().x * .5f, window.getSize().y * .15f }, "DEFAULT");
    Text::Init(sup_text, game.default_font, res_scalar * 24, { window.getSize().x * .5f, window.getSize().y * .2f }, "SUPPLEMENTARY DEFAULT");

    //What we do depends on our label
    switch (label) {
        //Menus
        case Menus::CHARCREA: {
            /*Things to determine:
            Race (aesthetics & mechanics),
            Class,
            Background,
            Ability scores,
            Skills,
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
            
            sf::Vector2f btn_pos = { window.getSize().x * .2f, window.getSize().y * .25f };
            float b_y_buffer = window.getSize().y*.1f;

            auto race_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::RACE_B, style_size });
            auto race_sm = make_unique<Menu>(game, window, scene, Menus::CCRACE);

            btn_pos.y += b_y_buffer;
            auto bg_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::BACKGROUND_B, style_size });
            auto bg_sm = make_unique<Menu>(game, window, scene, Menus::CCBG);

            btn_pos.y += b_y_buffer;
            auto class_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::CLASS, style_size });
            auto class_sm = make_unique<Menu>(game, window, scene, Menus::CCCLASS);

            btn_pos.y += b_y_buffer;
            auto ab_sc_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::AS, style_size });
            auto ab_sc_sm = make_unique<Menu>(game, window, scene, Menus::CCAS);

            btn_pos.y += b_y_buffer;
            auto skills_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SKILLS, style_size });
            auto skills_sm = make_unique<Menu>(game, window, scene, Menus::CCSKILLS);

            btn_pos.y += b_y_buffer;
            auto equip_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::EQUIPMENT_CC, style_size });
            auto equip_sm = make_unique<Menu>(game, window, scene, Menus::CCEQUIP);


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

            ui_elems.insert(make_pair(UIElems::RACE_B, race_btn));
            ui_elems.insert(make_pair(UIElems::BACKGROUND_B, bg_btn));
            ui_elems.insert(make_pair(UIElems::CLASS, class_btn));
            ui_elems.insert(make_pair(UIElems::AS, ab_sc_btn));
            ui_elems.insert(make_pair(UIElems::SKILLS, skills_btn));
            ui_elems.insert(make_pair(UIElems::EQUIPMENT_CC, equip_btn));

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
            //Menu text
            Text::SetCharSize(menu_text, res_scalar * 16);
            menu_text.setPosition({ window.getSize().x * .5f, window.getSize().y * .2f });
            Text::SetStr(menu_text, "Determine Your Ability Scores");
            //Supp. text
            Text::SetCharSize(sup_text, res_scalar * 12);
            sup_text.setPosition({ 0.f, 0.f});
            Text::SetStr(sup_text, "");

            //Pickers
            sf::Vector2f pkr_pos = { window.getSize().x * .5f, window.getSize().y * .2f };
            float p_y_buffer = window.getSize().y * .08f;

            auto str_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASSTR, style_size });

            pkr_pos.y += p_y_buffer;
            auto con_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASCON, style_size });

            pkr_pos.y += p_y_buffer;
            auto agi_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASAGI, style_size });

            pkr_pos.y += p_y_buffer;
            auto dex_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASDEX, style_size });

            pkr_pos.y += p_y_buffer;
            auto int_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASINT, style_size });

            pkr_pos.y += p_y_buffer;
            auto wis_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASWIS, style_size });

            pkr_pos.y += p_y_buffer;
            auto cha_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASCHA, style_size });

            ui_elems.insert(make_pair(UIElems::ASSTR, str_pkr));
            ui_elems.insert(make_pair(UIElems::ASCON, con_pkr));
            ui_elems.insert(make_pair(UIElems::ASAGI, agi_pkr));
            ui_elems.insert(make_pair(UIElems::ASDEX, dex_pkr));
            ui_elems.insert(make_pair(UIElems::ASINT, int_pkr));
            ui_elems.insert(make_pair(UIElems::ASWIS, wis_pkr));
            ui_elems.insert(make_pair(UIElems::ASCHA, cha_pkr));

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
            //Ability Scores: STR, CON, AGI, DEX, INT, WIS, CHA
            //Menu text
            Text::SetCharSize(menu_text, res_scalar * 16);
            menu_text.setPosition({ window.getSize().x * .5f, window.getSize().y * .2f });
            Text::SetStr(menu_text, "Pick Your Class");
            //Supp. text
            Text::SetCharSize(sup_text, res_scalar * 12);
            Text::SetOrigin(sup_text, { .5f, .0f });
            sup_text.setPosition({ window.getSize().x * .5f, window.getSize().y * .23f });
            Text::SetStr(sup_text, "Your Class determines your role in combat and the abilities you will learn as you progress.", window.getSize().x * .33f);

            //Pickers
            sf::Vector2f pkr_pos = { window.getSize().x * .5f, window.getSize().y * .2f };
            float p_y_buffer = window.getSize().y * .08f;

            auto str_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASSTR, style_size });
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
            Text::SetStr(menu_text, "Pick Your Race");
            //Supp. text
            Text::SetCharSize(sup_text, res_scalar * 12);
            Text::SetOrigin(sup_text, { .5f, .0f });
            sup_text.setPosition({ window.getSize().x * .5f, window.getSize().y * .23f });
            Text::SetStr(sup_text, "Your Race influences your appearance, some of your capabilities, and how others react to you.", window.getSize().x * .33f);

            sf::Vector2f pkr_pos = { window.getSize().x * .5f, window.getSize().y * .4f };
            float p_y_buffer = window.getSize().y * .1f;

            auto race_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::RACE_P, style_size });

            pkr_pos.y += p_y_buffer;
            auto size_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SIZE, style_size });

            pkr_pos.y += p_y_buffer;
            auto sex_pkr = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SEX, style_size });

            ui_elems.insert(make_pair(UIElems::RACE_P, race_pkr));
            ui_elems.insert(make_pair(UIElems::SIZE, size_pkr));
            ui_elems.insert(make_pair(UIElems::SEX, sex_pkr));

            break;
        }

        case Menus::CCSKILLS: {
            //Action Skills (pickers)
            //Tool Skills (pickers)
            //Weapon skill(s) (toggles)
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
            Text::SetStr(sup_text, "An Iron & Aether Adventure");

            sf::Vector2f btn_pos = { window.getSize().x * .5f, window.getSize().y * .4f };
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
            auto tutorial_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::TUTORIAL, style_size });

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
            ui_elems.insert(make_pair(UIElems::TUTORIAL, tutorial_btn));
            ui_elems.insert(make_pair(UIElems::OPTIONS, options_btn));
            ui_elems.insert(make_pair(UIElems::QUIT, quit_btn));
            break;
        }

        case Menus::OPTIONS: {
            auto music_sldr = make_shared<Slider>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Slider", 64, 9 },
                Animation::Transform{ {window.getSize().x*.5f, window.getSize().y*.5f}, ui_ori, ui_size},
                UI::Style{ UIElems::MUSIC_V, style_size });

            ui_elems.insert(make_pair(UIElems::MUSIC_V, music_sldr));

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

void Menu::OpenSM(const Menus s_m) {
    if (sub_menus.count(s_m) > 0) {
        sub_menus[s_m]->Open();
    }
    else cout << "That Sub-Menu does not exist in this Menu!" << endl;
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
        sf::Vector2f new_pos = { .0f, .0f };
        switch (ui.first) {
            case UIElems::APPLY:
                new_pos = { w_x * .5f, w_y * .8f };
            break;

            case UIElems::BACK:
                new_pos = { w_x * .5f, w_y * .9f };
            break;

            case UIElems::FULLSCREEN:
                new_pos = { w_x * (.5f+res_scalar*.02f), w_y * .675f};
            break;

            case UIElems::MUSIC_V:
                new_pos = { w_x * .5f, w_y * .3f };
            break;

            case UIElems::OPTIONS:
                new_pos = { w_x * .5f, w_y * .6f };
            break;

            case UIElems::QUIT:
                new_pos = { w_x * .5f, w_y * .7f };
            break;

            case UIElems::RESOLUTION:
                new_pos = { w_x * .5f, w_y * .55f };
            break;

            case UIElems::SFX_V:
                new_pos = { w_x * .5f, w_y * .425f };
            break;
        }
        u->SetBBox();
        u->MoveTo(new_pos);
    }
}

bool Menu::CheckUIElem(const UIElems ui) {
    if (ui_elems.find(ui) != ui_elems.end()) return true;
    
    cout << "That UI Element does not exist!" << endl;
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
