#include "Menu.h"
#include "Game.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(Game& g, sf::RenderWindow& w, Scene& s, Menus init_label) :
            game(g), window(w), scene(s), label(init_label), menu_text(game.default_font) {
    
    float res_scalar = game.GetResScale();
    int style_size = res_scalar * 12;

    Text::Init(menu_text, game.default_font, res_scalar * 36, { window.getSize().x * .5f, window.getSize().y * .3f }, "DEFAULT");
    
    //What we do depends on our label
    switch (label) {
        case Menus::CHARCREA: {
            /*Things to determine: (NOT CURRENTLY IN ORDER)
            Race/Racial features (aesthetics + mechanics),
            Sex,
            Class,
            Background,
            Skills,
            Ability scores,
            Starting equipment,
            Name
            */
            Text::SetCharSize(menu_text, res_scalar * 24);
            menu_text.setPosition({ window.getSize().x * .5f, window.getSize().y * .1f });
            Text::SetStr(menu_text, "Create Your Party");
            //Create a second text object that says "Character 1/2/3/4" - TO-DO
            
            auto race_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .25f, window.getSize().y * .3f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::RACE, style_size });
            auto size_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .25f, window.getSize().y * .4f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::SIZE, style_size });
            auto sex_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .25f, window.getSize().y * .5f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::SEX, style_size });
            auto class_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .25f, window.getSize().y * .6f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::CLASS, style_size });
            auto bg_picker = make_shared<Picker>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .25f, window.getSize().y * .7f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::BACKGROUND, style_size });
            //TO-DO: SKILLS, ABILITY SCORES, EQUIPMENT


            auto create_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .5f, window.getSize().y * .8f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::CREATE, style_size });
            auto back_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .5f, window.getSize().y * .8f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::BACK, style_size });

            ui_elems.insert(make_pair(UIElems::RACE, race_picker));
            ui_elems.insert(make_pair(UIElems::SIZE, size_picker));
            ui_elems.insert(make_pair(UIElems::SEX, sex_picker));
            ui_elems.insert(make_pair(UIElems::CLASS, class_picker));
            ui_elems.insert(make_pair(UIElems::BACKGROUND, bg_picker));
            ui_elems.insert(make_pair(UIElems::CREATE, create_btn));
            ui_elems.insert(make_pair(UIElems::BACK, back_btn));
            break;
        }

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

            auto char_crea_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .5f, window.getSize().y * .5f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::CHARCREA, style_size });
            auto load_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .5f, window.getSize().y * .6f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::LOAD, style_size });
            auto options_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .5f, window.getSize().y * .7f, .5f, .5f, res_scalar },
                UI::Style{ UIElems::OPTIONS, style_size });
            auto quit_btn = make_shared<Button>(
                Structure{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ window.getSize().x * .5f, window.getSize().y * .8f, .5f, .5f, res_scalar },
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