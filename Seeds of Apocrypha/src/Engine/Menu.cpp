#include "Menu.h"
#include "Game.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(Game& g, sf::RenderWindow& w, Scene& s, Menus init_label) :
            game(g), window(w), scene(s), label(init_label), menu_text(game.default_font), sup_text(game.default_font) {
    
    float res_scalar = game.GetResScale();
    int style_size = res_scalar * 14;
    ui_size = { res_scalar, res_scalar };

    //Menu and Supp text variables
    uint m_t_size = res_scalar * 36;
    sf::Vector2f m_t_pos = { .0f, .0f };
    string m_t_str = "MENU DEFAULT";
    uint s_t_size = res_scalar * 24;
    sf::Vector2f s_t_pos = { .0f, .0f };
    string s_t_str = "SUPPLEMENTARY DEFAULT";
    float s_t_str_max_w = -1;

    Text::Init(sup_text, game.default_font, res_scalar * 24, { window.getSize().x * .5f, window.getSize().y * .225f });
    Text::SetOrigin(sup_text, { .5f, .0f });

    //What we do depends on our label
    switch (label) {
        //Menus
        case Menus::CHARCREA: {
            m_t_size = res_scalar * 24; m_t_pos = { window.getSize().x * .5f, window.getSize().y * .1f }; m_t_str = "Create Your Party";
            s_t_size = res_scalar * 18; s_t_pos = { window.getSize().x * .5f, window.getSize().y * .13f }; s_t_str = "Use the options below to create your party of 4 adventurers";
            s_t_str_max_w = window.getSize().x * .33f;
            
            sf::Vector2f btn_pos = { window.getSize().x * .2f, window.getSize().y * .25f };
            float b_y_buffer = window.getSize().y*.1f;

            auto btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::RACE_B, style_size });
            auto sm = make_unique<Menu>(game, window, scene, Menus::CCRACE);
            ui_elems.insert({ UIElems::RACE_B, btn });
            sub_menus.insert({ Menus::CCRACE, move(sm) });

            btn_pos.y += b_y_buffer;
            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::BACKGROUND_B, style_size });
            sm = make_unique<Menu>(game, window, scene, Menus::CCBG);
            ui_elems.insert({ UIElems::BACKGROUND_B, btn });
            sub_menus.insert({ Menus::CCBG, move(sm) });

            btn_pos.y += b_y_buffer;
            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::CLASS, style_size });
            sm = make_unique<Menu>(game, window, scene, Menus::CCCLASS);
            ui_elems.insert({ UIElems::CLASS, btn });
            sub_menus.insert({ Menus::CCCLASS, move(sm) });

            btn_pos.y += b_y_buffer;
            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::AS, style_size });
            sm = make_unique<Menu>(game, window, scene, Menus::CCAS);
            ui_elems.insert({ UIElems::AS, btn });
            sub_menus.insert({ Menus::CCAS, move(sm) });

            btn_pos.y += b_y_buffer;
            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SKILLS, style_size });
            sm = make_unique<Menu>(game, window, scene, Menus::CCSKILLS);
            ui_elems.insert({ UIElems::SKILLS, btn });
            sub_menus.insert({ Menus::CCSKILLS, move(sm) });

            btn_pos.y += b_y_buffer;
            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::EQUIPMENT_CC, style_size });
            sm = make_unique<Menu>(game, window, scene, Menus::CCEQUIP);
            ui_elems.insert({ UIElems::EQUIPMENT_CC, btn });
            sub_menus.insert({ Menus::CCEQUIP, move(sm) });


            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ {window.getSize().x * .5f, window.getSize().y * .8f}, ui_ori, ui_size },
                UI::Style{ UIElems::CREATE, style_size });
            ui_elems.insert({ UIElems::CREATE, btn });
            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ {window.getSize().x * .5f, window.getSize().y * .9f}, ui_ori, ui_size },
                UI::Style{ UIElems::BACK, style_size });
            ui_elems.insert({ UIElems::BACK, btn });

            break;
        }

        //Character Creation Sub-Menus
        case Menus::CCAS: {
            //Ability Scores: STR, CON, AGI, DEX, INT, WIS, CHA
            m_t_size = res_scalar * 16; m_t_pos = { window.getSize().x * .5f, window.getSize().y * .2f }; m_t_str = "Determine Your Ability Scores";
            s_t_size = res_scalar * 12; s_t_pos = { window.getSize().x * .5f, window.getSize().y * .23f }; s_t_str = "Your Ability Scores --";
            s_t_str_max_w = window.getSize().x * .33f;

            //Pickers
            sf::Vector2f pkr_pos = { window.getSize().x * .5f, window.getSize().y * .2f };
            float p_y_buffer = window.getSize().y * .08f;

            auto str_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASSTR, style_size });

            pkr_pos.y += p_y_buffer;
            auto con_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASCON, style_size });

            pkr_pos.y += p_y_buffer;
            auto agi_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASAGI, style_size });

            pkr_pos.y += p_y_buffer;
            auto dex_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASDEX, style_size });

            pkr_pos.y += p_y_buffer;
            auto int_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASINT, style_size });

            pkr_pos.y += p_y_buffer;
            auto wis_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::ASWIS, style_size });

            pkr_pos.y += p_y_buffer;
            auto cha_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
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
            m_t_size = res_scalar * 16; m_t_pos = { window.getSize().x * .5f, window.getSize().y * .2f }; m_t_str = "Pick Your Class";
            s_t_size = res_scalar * 12; s_t_pos = { window.getSize().x * .5f, window.getSize().y * .23f }; s_t_str = "Your Class determines your role in combat and the abilities you will learn as you progress";
            s_t_str_max_w = window.getSize().x * .33f;

            //Pickers
            sf::Vector2f pkr_pos = { window.getSize().x * .5f, window.getSize().y * .2f };
            float p_y_buffer = window.getSize().y * .08f;

            /*auto str_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::CLASS, style_size });*/
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
            m_t_size = res_scalar * 16; m_t_pos = { window.getSize().x * .5f, window.getSize().y * .2f }; m_t_str = "Pick Your Race";
            s_t_size = res_scalar * 12; s_t_pos = { window.getSize().x * .5f, window.getSize().y * .23f }; s_t_str = "Your Race influences your appearance, some of your capabilities, and how others react to you";
            s_t_str_max_w = window.getSize().x * .33f;

            sf::Vector2f pkr_pos = { window.getSize().x * .5f, window.getSize().y * .4f };
            float p_y_buffer = window.getSize().y * .1f;

            auto race_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::RACE_P, style_size });

            pkr_pos.y += p_y_buffer;
            auto size_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ pkr_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SIZE, style_size });

            pkr_pos.y += p_y_buffer;
            auto sex_pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
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

        case Menus::MAIN: {
            m_t_size = res_scalar * 36; m_t_pos = { window.getSize().x * .5f, window.getSize().y * .15f }; m_t_str = "Seeds of Apocrypha";
            s_t_size = res_scalar * 24; s_t_pos = { window.getSize().x * .5f, window.getSize().y * .225f }; s_t_str = "An Iron & Aether Adventure";

            sf::Vector2f btn_pos = { window.getSize().x * .5f, window.getSize().y * .4f };
            float b_y_buffer = window.getSize().y * .1f;

            auto char_crea_btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::CHARCREA, style_size });
            
            btn_pos.y += b_y_buffer;
            auto load_btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::LOAD, style_size });
            
            btn_pos.y += b_y_buffer;
            auto tutorial_btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::TUTORIAL, style_size });

            btn_pos.y += b_y_buffer;
            auto options_btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ btn_pos, ui_ori, ui_size },
                UI::Style{ UIElems::OPTIONS, style_size });
            
            btn_pos.y += b_y_buffer;
            auto quit_btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
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
            m_t_size = res_scalar * 36; m_t_pos = { window.getSize().x * .5f, window.getSize().y * .15f }; m_t_str = "Seeds of Apocrypha";
            s_t_size = res_scalar * 24; s_t_pos = { window.getSize().x * .5f, window.getSize().y * .2f }; s_t_str = "Options";

            sf::Vector2f elem_pos = { window.getSize().x * .5f, window.getSize().y * .3f };
            float elem_y_buffer = window.getSize().y * .09f;

            //Music and sfx sliders
            auto sldr = make_shared<Slider>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Slider", 192, 27 },
                Animation::Transform{ elem_pos, ui_ori, ui_size},
                UI::Style{ UIElems::MUSIC_V, style_size });
            ui_elems.insert({ UIElems::MUSIC_V, sldr });

            elem_pos.y += elem_y_buffer;
            sldr = make_shared<Slider>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Slider", 192, 27 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SFX_V, style_size });
            ui_elems.insert({ UIElems::SFX_V, sldr });


            //Resolution picker, fullscreen toggle, and apply button
            elem_pos.y += elem_y_buffer;
            auto pkr = make_shared<Picker>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::RESOLUTION, style_size });
            ui_elems.insert({ UIElems::RESOLUTION, pkr });

            elem_pos.y += elem_y_buffer;
            float f_t_x = elem_pos.x + window.getSize().x * (res_scalar * .01);
            auto tgl = make_shared<Toggle>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Toggle", 24, 24, 0, 2 },
                Animation::Transform{ {f_t_x, elem_pos.y}, ui_ori, ui_size },
                UI::Style{ UIElems::FULLSCREEN, style_size });
            ui_elems.insert({ UIElems::FULLSCREEN, tgl });

            elem_pos.y += elem_y_buffer;
            auto btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::APPLY, style_size });
            ui_elems.insert({ UIElems::APPLY, btn });


            //Back button
            elem_pos.y = window.getSize().y * .9;
            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::BACK, style_size });
            ui_elems.insert({ UIElems::BACK, btn });

            break;
        }

        case Menus::OPTIONS_G: {
            m_t_size = res_scalar * 18; m_t_pos = { window.getViewport(game.camera).position.x + window.getViewport(game.camera).size.x*.5f, window.getViewport(game.camera).position.y + window.getViewport(game.camera).size.y*.2f}; m_t_str = "Options";
            s_t_size = res_scalar * 12; s_t_pos = { window.getView().getCenter().x, window.getView().getViewport().position.y + window.getView().getViewport().size.y * .12f}; s_t_str = "Adjust Settings";
            if (game.debug) {
                m_t_pos = { window.getViewport(game.hud).position.x + window.getViewport(game.hud).size.x*.5f, window.getViewport(game.hud).position.y + window.getViewport(game.hud).size.y*.2f};
                s_t_pos = { window.getView().getCenter().x, window.getView().getViewport().position.y + window.getView().getViewport().size.y * .12f};
            }


            sf::Vector2f elem_pos = { window.getView().getCenter().x, window.getView().getViewport().position.y + window.getView().getViewport().size.y * .3f };
            float elem_y_buffer = window.getView().getViewport().size.y* .09f;

            //Close button
            auto btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::CLOSE, style_size });
            ui_elems.insert({ UIElems::CLOSE, btn });

            //Adjust camera speed - TO-DO
            
            //Return to Title
            elem_pos.y += elem_y_buffer;
            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::TITLE, style_size });
            ui_elems.insert({ UIElems::TITLE, btn });

            //Quit
            elem_pos.y += elem_y_buffer;
            btn = make_shared<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::QUIT, style_size });
            ui_elems.insert({ UIElems::QUIT, btn });
            break;
        }
    }
    
    //Initialize our texts
    Text::Init(menu_text, game.default_font, m_t_size, m_t_pos, m_t_str);
    Text::Init(sup_text, game.default_font, s_t_size, s_t_pos, s_t_str, {.5f, .0f});
    if (s_t_str_max_w != -1)
        Text::SetStr(sup_text, s_t_str, s_t_str_max_w);


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

    //A more efficient way to do this would be to base the new size on the old size and do some funky math shit with the scalar, but this works for now
    uint new_m_t_size = res_scalar;
    sf::Vector2f new_m_t_pos = sf::Vector2f(window.getSize()) * .5f;
    uint new_s_t_size = res_scalar;
    sf::Vector2f new_s_t_pos = sf::Vector2f(window.getSize()) * .5f;
    switch (label) {
        case Menus::CHARCREA:
            new_m_t_size = res_scalar * 24;
            new_m_t_pos.y = window.getSize().y * .1f;
            new_s_t_size = res_scalar * 18;
            new_s_t_pos.y = window.getSize().y * .13f;
        break;

        case Menus::CCAS:
        case Menus::CCBG:
        case Menus::CCCLASS:
        case Menus::CCEQUIP:
        case Menus::CCRACE:
        case Menus::CCSKILLS:
            new_m_t_size = res_scalar * 16;
            new_m_t_pos.y = window.getSize().y * .2f;
            new_s_t_size = res_scalar * 12;
            new_s_t_pos.y = window.getSize().y * .23f;
        break;

        case Menus::MAIN:
        case Menus::OPTIONS:
            new_m_t_size = res_scalar * 36;
            new_m_t_pos.y = window.getSize().y * .15f;
            new_s_t_size = res_scalar * 24;
            new_s_t_pos.y = window.getSize().y * .2f;
        break;
    }
    Text::SetCharSize(menu_text, new_m_t_size);
    Text::SetOrigin(menu_text);
    menu_text.setPosition(new_m_t_pos);
    Text::SetCharSize(sup_text, new_s_t_size);
    Text::SetOrigin(sup_text, { .5f, .0f });
    sup_text.setPosition(new_s_t_pos);


    //UI Elements
    sf::Vector2f new_pos = sf::Vector2f(window.getSize())*.5f;
    for (const auto& ui : ui_elems) {
        auto& u = ui.second;

        u->Resize(res_scalar);

        //Adjust the positions
        switch (ui.first) {
            case UIElems::APPLY:
                new_pos.x = window.getSize().x * .5f;
                new_pos.y = window.getSize().y * .66f;
            break;

            case UIElems::BACK:
                new_pos.x = window.getSize().x * .5f;
                new_pos.y = window.getSize().y * .9f;
            break;

            case UIElems::CHARCREA:
                new_pos.y = window.getSize().y * .4f;
            break;

            case UIElems::FULLSCREEN:
                new_pos = { window.getSize().x * (.5f+res_scalar*.01f), window.getSize().y * .57f};
            break;

            case UIElems::LOAD:
                new_pos.y = window.getSize().y * .5f;
            break;

            case UIElems::MUSIC_V:
                new_pos.y = window.getSize().y * .3f;
            break;

            case UIElems::OPTIONS:
                new_pos.y = window.getSize().y * .7f;
            break;

            case UIElems::QUIT:
                new_pos.y = window.getSize().y * .8f;
            break;

            case UIElems::RESOLUTION:
                new_pos.y = window.getSize().y * .48f;
            break;

            case UIElems::SFX_V:
                new_pos.y = window.getSize().y * .39f;
            break;

            case UIElems::TUTORIAL:
                new_pos.y = window.getSize().y * .6f;
            break;
        }
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
