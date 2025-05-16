#include "Menu.h"
#include "Scene.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(Game& g, Scene& s, const Menus init_label) :
            game(g), scene(s), label(init_label) {

    //Putting these here instead of the initializer list because for some reason it doesn't work when I put them there - Menu just doesn't seem to interface with Game very well?
    menu_text = make_u<Text>(game.default_font72.get());
    sup_text = make_u<Text>(game.default_font48.get());
    res_scalar = game.GetResScale();
    ui_scale = { (int)res_scalar };
    Text::Info* mti = &menu_text->info;
    Text::Info* sti = &sup_text->info;

    //Menu and Supp text variables
    menu_text->SetOrigin();
    Vector2i m_t_pos = { 0, 0 };
    string m_t_str = "MENU DEFAULT";

    sup_text->SetOrigin();
    Vector2i s_t_pos = { 0, 0 };
    string s_t_str = "SUPPLEMENTARY DEFAULT";
    float s_t_str_max_w = -1;
    
    Sprite::Info elem_info = {}; elem_info.origin = ui_ori; elem_info.scale = ui_scale;
    float e_y_buffer = 0;
    //What we do depends on our label
    switch (label) {
        
        //Menus
        case Menus::CHARCREA: {
            m_t_pos = Vector2i(game.GetResolution().x * .5, game.GetResolution().y * .1); m_t_str = "Create Your Party";
            s_t_pos = Vector2i(m_t_pos.x, game.GetResolution().y * .13); s_t_str = "Use the options below to create your party of 4 adventurers";
            s_t_str_max_w = game.GetResolution().x * .33;
            
            elem_info.pos = Vector2i(round(game.GetResolution().x * .2), round(game.GetResolution().y * .25));
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };

            e_y_buffer = round(game.GetResolution().y * .1f);

            auto btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::RACE_B);
            auto sm = make_u<Menu>(game, scene, Menus::CCRACE);
            ui_elems.insert({ UIElems::RACE_B, btn });
            sub_menus.insert({ Menus::CCRACE, move(sm) });

            elem_info.pos.y += e_y_buffer; //.35
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::BACKGROUND_B);
            sm = make_u<Menu>(game, scene, Menus::CCBG);
            ui_elems.insert({ UIElems::BACKGROUND_B, btn });
            sub_menus.insert({ Menus::CCBG, move(sm) });

            elem_info.pos.y += e_y_buffer; //.45
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::CLASS_B);
            sm = make_u<Menu>(game, scene, Menus::CCCLASS);
            ui_elems.insert({ UIElems::CLASS_B, btn });
            sub_menus.insert({ Menus::CCCLASS, move(sm) });

            elem_info.pos.y += e_y_buffer; //.55
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::AS);
            sm = make_u<Menu>(game, scene, Menus::CCAS);
            ui_elems.insert({ UIElems::AS, btn });
            sub_menus.insert({ Menus::CCAS, move(sm) });

            elem_info.pos.y += e_y_buffer; //.65
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::SKILLS);
            sm = make_u<Menu>(game, scene, Menus::CCSKILLS);
            ui_elems.insert({ UIElems::SKILLS, btn });
            sub_menus.insert({ Menus::CCSKILLS, move(sm) });

            elem_info.pos.y += e_y_buffer; //.75
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::EQUIPMENT_CC);
            sm = make_u<Menu>(game, scene, Menus::CCEQUIP);
            ui_elems.insert({ UIElems::EQUIPMENT_CC, btn });
            sub_menus.insert({ Menus::CCEQUIP, move(sm) });


            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::CREATE);
            ui_elems.insert({ UIElems::CREATE, btn });
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::BACK);
            ui_elems.insert({ UIElems::BACK, btn });

            break;
        }

        //Character Creation Sub-Menus
        case Menus::CCAS: {
            //Ability Scores: STR, CON, AGI, DEX, INT, WIS, CHA
            //Apparently need to decrease m_t & s_t font sizes (TO-DO)
            m_t_pos = Vector2i(game.GetResolution().x * .5, game.GetResolution().y * .2); m_t_str = "Determine Your Ability Scores";
            s_t_pos = Vector2i(game.GetResolution().x * .5, game.GetResolution().y * .23); s_t_str = "Your Ability Scores --";
            s_t_str_max_w = game.GetResolution().x * .33;

            elem_info.pos = Vector2i(round(game.GetResolution().x * .5), round(game.GetResolution().y * .2));
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };

            e_y_buffer = round(game.GetResolution().y * .08f);

            auto pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::ASSTR);
            ui_elems.insert({ UIElems::ASSTR, pkr });

            elem_info.pos.y += e_y_buffer;
            pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::ASCON);
            ui_elems.insert({ UIElems::ASCON, pkr });

            elem_info.pos.y += e_y_buffer;
            pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::ASAGI);
            ui_elems.insert({ UIElems::ASAGI, pkr });

            elem_info.pos.y += e_y_buffer;
            pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::ASDEX);
            ui_elems.insert({ UIElems::ASDEX, pkr });

            elem_info.pos.y += e_y_buffer;
            pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::ASINT);
            ui_elems.insert({ UIElems::ASINT, pkr });

            elem_info.pos.y += e_y_buffer;
            pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::ASWIS);
            ui_elems.insert({ UIElems::ASWIS, pkr });

            elem_info.pos.y += e_y_buffer;
            pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::ASCHA);
            ui_elems.insert({ UIElems::ASCHA, pkr });

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
            //Adjust font sizes - TO-DO
            m_t_pos = Vector2i(game.GetResolution().x * .5f, game.GetResolution().y * .2f); m_t_str = "Pick Your Class";
            s_t_pos = Vector2i(game.GetResolution().x * .5f, game.GetResolution().y * .23f); s_t_str = "Your Class determines your role in combat and the abilities you will learn as you progress";
            s_t_str_max_w = game.GetResolution().x * .33f;

            //Pickers
            elem_info.pos = Vector2i(round(game.GetResolution().x * .5), round(game.GetResolution().y * .2));
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };

            e_y_buffer = round(game.GetResolution().y * .08f);

            auto pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::CLASS_P);
            ui_elems.insert({ UIElems::CLASS_P, pkr });
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
            //Adjust font size - TO-DO
            m_t_pos = Vector2i(game.GetResolution().x * .5, game.GetResolution().y * .2); m_t_str = "Pick Your Race";
            s_t_pos = Vector2i(game.GetResolution().x * .5, game.GetResolution().y * .23); s_t_str = "Your Race influences your appearance, some of your capabilities, and how others react to you";
            s_t_str_max_w = game.GetResolution().x * .33;

            //Pickers
            elem_info.pos = Vector2i(round(game.GetResolution().x * .5), round(game.GetResolution().y * .4));
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };

            e_y_buffer = round(game.GetResolution().y * .1f);

            auto pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::RACE_P);
            ui_elems.insert({ UIElems::RACE_P, pkr });

            elem_info.pos.y += e_y_buffer;
            pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::SIZE);
            ui_elems.insert({ UIElems::SIZE, pkr });

            elem_info.pos.y += e_y_buffer;
            pkr = make_s<Picker>(
                game, &scene, *this,
                elem_info,
                UIElems::SEX);
            ui_elems.insert({ UIElems::SEX, pkr });

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
            
            m_t_pos = Vector2i(round(game.GetResolution().x * .5f), round(game.GetResolution().y * .12f)); m_t_str = m_t_str = "Seeds of Apocrypha";
            s_t_pos = Vector2i(round(game.GetResolution().x * .5f), round(game.GetResolution().y * .15f)); s_t_str = "An Iron & Aether Adventure";

            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };
            elem_info.pos = Vector2i(round(game.GetResolution().x * .5f), round(game.GetResolution().y * .4f));

            e_y_buffer = round(game.GetResolution().y * .1f);

            auto btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::CHARCREA);
            ui_elems.insert(make_pair(UIElems::CHARCREA, btn));
            
            elem_info.pos.y += e_y_buffer;
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::LOAD);
            ui_elems.insert(make_pair(UIElems::LOAD, btn));
            
            elem_info.pos.y += e_y_buffer;
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::TUTORIAL);
            ui_elems.insert(make_pair(UIElems::TUTORIAL, btn));

            elem_info.pos.y += e_y_buffer;
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::OPTIONS);
            ui_elems.insert(make_pair(UIElems::OPTIONS, btn));
            
            elem_info.pos.y += e_y_buffer;
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::QUIT);
            ui_elems.insert(make_pair(UIElems::QUIT, btn));

            elem_info.pos = Vector2i(round(game.GetResolution().x * .75), round(game.GetResolution().y * .5));
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::DEBUG_ROOM);
            ui_elems.insert(make_pair(UIElems::DEBUG_ROOM, btn));
            
            break;
        }
        
        case Menus::OPTIONS: {
            m_t_pos = Vector2i(game.GetResolution().x * .5, game.GetResolution().y * .12); m_t_str = "Seeds of Apocrypha";
            s_t_pos = Vector2i(game.GetResolution().x * .5, game.GetResolution().y * .15); s_t_str = "Options";

            elem_info.pos = Vector2i(round(game.GetResolution().x * .5), round(game.GetResolution().y * .3));
            elem_info.sheet = "UI/Slider"; elem_info.frame_size = { 192, 27 };

            e_y_buffer = round(game.GetResolution().y * .09f);

            //Music and sfx sliders
            auto sldr = make_s<Slider>(
                game, &scene, *this,
                elem_info,
                UIElems::MUSIC_V);
            ui_elems.insert({ UIElems::MUSIC_V, sldr });

            elem_info.pos.y += e_y_buffer;
            sldr = make_s<Slider>(
                game, &scene, *this,
                elem_info,
                UIElems::SFX_V);
            ui_elems.insert({ UIElems::SFX_V, sldr });
            

            //Resolution picker, fullscreen toggle, and apply button
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };
            elem_info.pos.y += e_y_buffer;
            auto pkr = make_s<Picker>(
                game,&scene, *this,
                elem_info,
                UIElems::RESOLUTION);
            ui_elems.insert({ UIElems::RESOLUTION, pkr });


            elem_info.sheet = "UI/Toggle"; elem_info.frame_size = { 24, 24 };
            elem_info.pos.y += e_y_buffer;
            //float f_t_x = elem_info.pos.x + game.GetResolution().x * (res_scalar * .01);
            auto tgl = make_s<Toggle>(
                game, &scene, *this,
                elem_info,
                UIElems::FULLSCREEN);
            ui_elems.insert({ UIElems::FULLSCREEN, tgl });
            

            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };
            elem_info.pos.y += e_y_buffer;
            auto btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::APPLY);
            ui_elems.insert({ UIElems::APPLY, btn });
            

            //Back button
            elem_info.pos.y = round(game.GetResolution().y * .9f);
            btn = make_s<Button>(
                game, &scene, *this,
                elem_info,
                UIElems::BACK);
            ui_elems.insert({ UIElems::BACK, btn });

            break;
        }
        /*
        case Menus::OPTIONS_G: {
            Vector2f cam_top_left = game.camera.getCenter() - (game.camera.getSize() * .5f);
            m_t_size = res_scalar * 18; m_t_pos = Vector2i(game.camera.getCenter().x, round(cam_top_left.y + game.camera.getSize().y*.2)); m_t_str = "Options";
            s_t_size = res_scalar * 12; s_t_pos = Vector2i(game.camera.getCenter().x, round(cam_top_left.y + game.camera.getSize().y * .12)); s_t_str = "";
            
            Vector2i elem_pos = Vector2i(game.camera.getCenter().x, round(cam_top_left.y + game.camera.getSize().y*.28));
            uint elem_y_buffer = round(game.camera.getSize().y * .09);

            ui_size = Vector2u(round(ui_size.x * .5), round(ui_size.y * .5));

            //Music, sfx, and cam speed sliders
            auto sldr = make_s<Slider>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Slider", 192, 27 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::MUSIC_V, style_size });
            ui_elems.insert({ UIElems::MUSIC_V, sldr });

            elem_pos.y += elem_y_buffer;
            sldr = make_s<Slider>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Slider", 192, 27 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::SFX_V, style_size });
            ui_elems.insert({ UIElems::SFX_V, sldr });

            elem_pos.y += elem_y_buffer;
            sldr = make_s<Slider>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Slider", 192, 27 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::CAMSPD, style_size });
            ui_elems.insert({ UIElems::CAMSPD, sldr });

            //Buttons
            ui_size = { ui_size.x * 2, ui_size.y * 2 };
            //Close button
            elem_pos.y += elem_y_buffer;
            auto btn = make_s<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ elem_pos, ui_ori, ui_size},
                UI::Style{ UIElems::CLOSE, style_size });
            ui_elems.insert({ UIElems::CLOSE, btn });
            
            //Return to Title
            elem_pos.y += elem_y_buffer;
            btn = make_s<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::TITLE, style_size });
            ui_elems.insert({ UIElems::TITLE, btn });

            //Quit
            elem_pos.y += elem_y_buffer;
            btn = make_s<Button>(
                Engine{ game, window, &scene }, *this,
                AnimInfo{ "UI/Button", 93, 26 },
                Animation::Transform{ elem_pos, ui_ori, ui_size },
                UI::Style{ UIElems::QUIT, style_size });
            ui_elems.insert({ UIElems::QUIT, btn });
            break;
        }
        */
    }
    
    //Set our texts (not strictly necessary but keeping for now)
    menu_text->MoveTo(m_t_pos); mti->str = m_t_str; mti->max_width = game.GetResolution().x;
    sup_text->MoveTo(s_t_pos); sti->str = s_t_str; sup_text->SetOrigin({ .5f, .0f }); sti->max_width = game.GetResolution().x;
    if (s_t_str_max_w != -1)
        sti->max_width = s_t_str_max_w;

    //Add our UI elements to the scene entities vector
    for (const auto& ui : ui_elems)
        scene.AddEntity(ui.second);
}

void Menu::Update() {
    if (open) {
        Vector2i m_t_pos = { 0, 0 };
        Vector2i s_t_pos = { 500, 0 };
        Vector2i elem_pos = { 0, 0 };
        uint elem_y_buffer = 0;
        /*

        switch (label) {
            case Menus::OPTIONS_G:
                //Vector2f cam_top_left = game.camera.getCenter() - (game.camera.getSize() * .5f);
                //m_t_pos = Vector2i(game.camera.getCenter().x, round(cam_top_left.y + game.camera.getSize().y * .2));
                //s_t_pos = Vector2i(game.camera.getCenter().x, cam_top_left.y + game.camera.getSize().y * .12);
                //menu_text->info.pos = m_t_pos;
                //sup_text->info.pos = s_t_pos;

                elem_pos = Vector2i(game.camera.getCenter().x, round(cam_top_left.y + game.camera.getSize().y * .28));

                //This would be easier to do with a vector, but switching from a u_map to a vector at this point would just be a pain in the ass
                for (const auto& ui : ui_elems) {
                    switch (ui.first) {
                        case UIElems::MUSIC_V:
                            elem_pos.y = round(cam_top_left.y + game.camera.getSize().y * .28);
                        break;

                        case UIElems::SFX_V:
                            elem_pos.y = round(cam_top_left.y + game.camera.getSize().y * .37);
                        break;

                        case UIElems::CAMSPD:
                            elem_pos.y = round(cam_top_left.y + game.camera.getSize().y * .46);
                        break;

                        case UIElems::CLOSE:
                            elem_pos.y = round(cam_top_left.y + game.camera.getSize().y * .55);
                        break;

                        case UIElems::TITLE:
                            elem_pos.y = round(cam_top_left.y + game.camera.getSize().y * .64);
                        break;

                        case UIElems::QUIT:
                            elem_pos.y = round(cam_top_left.y + game.camera.getSize().y * .73);
                        break;
                    }
                    ui.second->MoveTo(elem_pos);
                }
            break;
        }

        */
        for (auto& s_m : sub_menus)
            s_m.second->Update();
    }

}

void Menu::Draw() {
    if (open) {
        game.renderer->DrawTxt(*menu_text);
        game.renderer->DrawTxt(*sup_text);
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
    if (sub_menus.count(s_m) > 0)
        sub_menus[s_m]->Open();
    else cout << "That Sub-Menu does not exist in this Menu!\n";
}

void Menu::Resize() {
    //Resize all text and UI elements according to the new res scalar and window dimensions
    float res_scalar = game.GetResScale();

    //A more efficient way to do this would be to base the new size on the old size and do some funky math shit with the scalar, but this works for now
    Vector2i new_m_t_pos = Vector2i(round(game.GetResolution().x * .5f), round(game.GetResolution().y * .5f));
    Vector2i new_s_t_pos = Vector2i(round(game.GetResolution().x * .5f), round(game.GetResolution().y * .5f));
    switch (label) {
        case Menus::CHARCREA:
            new_m_t_pos.y = game.GetResolution().y * .1f;
            new_s_t_pos.y = game.GetResolution().y * .13f;
        break;

        case Menus::CCAS:
        case Menus::CCBG:
        case Menus::CCCLASS:
        case Menus::CCEQUIP:
        case Menus::CCRACE:
        case Menus::CCSKILLS:
            new_m_t_pos.y = game.GetResolution().y * .2f;
            new_s_t_pos.y = game.GetResolution().y * .23f;
        break;

        case Menus::MAIN:
        case Menus::OPTIONS:
            new_m_t_pos.y = game.GetResolution().y * .15f;
            new_s_t_pos.y = game.GetResolution().y * .2f;
        break;
    }

    menu_text->SetOrigin();
    menu_text->info.pos = new_m_t_pos;
    sup_text->SetOrigin({.5f, .0f});
    sup_text->info.pos = new_s_t_pos;

    //UI Elements
    Vector2i new_pos = Vector2i(round(game.GetResolution().x*.5f), round(game.GetResolution().y*.5f));
    
    for (const auto& ui : ui_elems) {
        auto& u = ui.second;

        u->Resize(res_scalar);

        //Adjust the positions - there has to be a more efficient goddamn way of doing this
        switch (ui.first) {
            case UIElems::APPLY:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .66f;
            break;

            case UIElems::AS:
                new_pos.x = game.GetResolution().x * .2f;
                new_pos.y = game.GetResolution().y * .55f;
            break;

            case UIElems::ASAGI:
                new_pos.x = game.GetResolution().x * .5;
                new_pos.y = game.GetResolution().y * .36;
            break;

            case UIElems::ASCHA:
                new_pos.x = game.GetResolution().x * .5;
                new_pos.y = game.GetResolution().y * .68;
            break;

            case UIElems::ASCON:
                new_pos.x = game.GetResolution().x * .5;
                new_pos.y = game.GetResolution().y * .28;
            break;

            case UIElems::ASDEX:
                new_pos.x = game.GetResolution().x * .5;
                new_pos.y = game.GetResolution().y * .44;
            break;

            case UIElems::ASINT:
                new_pos.x = game.GetResolution().x * .5;
                new_pos.y = game.GetResolution().y * .52;
            break;

            case UIElems::ASSTR:
                new_pos.x = game.GetResolution().x * .5;
                new_pos.y = game.GetResolution().y * .2;
            break;

            case UIElems::ASWIS:
                new_pos.x = game.GetResolution().x * .5;
                new_pos.y = game.GetResolution().y * .6;
            break;

            case UIElems::BACK:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .9f;
            break;
            
            case UIElems::BACKGROUND_B:
                new_pos.x = game.GetResolution().x * .2f;
                new_pos.y = game.GetResolution().y * .35f;
            break;

            case UIElems::CHARCREA:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .4f;
            break;

            case UIElems::CLASS_B:
                new_pos.x = game.GetResolution().x * .2f;
                new_pos.y = game.GetResolution().y * .45f;
            break;

            case UIElems::CREATE:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .8f;
            break;

            case UIElems::DEBUG_ROOM:
                new_pos.x = game.GetResolution().x * .75f;
                new_pos.y = game.GetResolution().y * .5f;
            break;

            case UIElems::EQUIPMENT_CC:
                new_pos.x = game.GetResolution().x * .2f;
                new_pos.y = game.GetResolution().y * .75f;
            break;

            case UIElems::FULLSCREEN: {
                new_pos = Vector2i(round(game.GetResolution().x * (.5f + res_scalar * .01f)), round(game.GetResolution().y * .57));
                string new_status = "False";
                if (game.GetResolution() == game.window->ScreenSize())
                    new_status = "True";

                SetUIElemStatus(ui.first, new_status);
            } break;

            case UIElems::LOAD:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .5f;
            break;

            case UIElems::MUSIC_V:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .3f;
            break;

            case UIElems::OPTIONS:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .7f;
            break;

            case UIElems::QUIT:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .8f;
            break;

            case UIElems::RACE_B:
                new_pos.x = game.GetResolution().x * .2f;
                new_pos.y = game.GetResolution().y * .25f;
            break;

            case UIElems::RESOLUTION:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .48f;
            break;

            case UIElems::SFX_V:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .39f;
            break;

            case UIElems::SKILLS:
                new_pos.x = game.GetResolution().x * .2f;
                new_pos.y = game.GetResolution().y * .65f;
            break;

            case UIElems::TUTORIAL:
                new_pos.x = game.GetResolution().x * .5f;
                new_pos.y = game.GetResolution().y * .6f;
            break;
        }

        u->MoveTo(new_pos);
    }
}

void Menu::AddUIElem(const s_ptr<UI> new_elem) {
    ui_elems.insert({ new_elem.get()->GetElem(), new_elem });
    scene.AddEntity(new_elem);
}

void Menu::RemoveUIElem(const UIElems ui) {
    if (CheckUIElem(ui)) {
        ui_elems[ui].get()->alive = false;
        ui_elems[ui].reset();
        ui_elems.erase(ui);
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
        else if (auto toggle = dynamic_cast<Toggle*>(ui_elems[ui].get())) {
            if (new_status == "True")
                toggle->on = true;
            else
                toggle->on = false;
        }
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
            if (toggle->on) return "True";
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
