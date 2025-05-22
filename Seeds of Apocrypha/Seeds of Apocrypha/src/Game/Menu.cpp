#include "Menu.h"
#include "Scene.h" //Game and all its BS
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(const Menus init_label) :
    scene(*game->active_scene), label(init_label) {

    //Putting these here instead of the initializer list because for some reason it doesn't work when I put them there - Menu just doesn't seem to interface with Game very well?
    menu_text.Init(&game->default_fonts[72]);
    sup_text.Init(&game->default_fonts[48]);
    res_scalar = game->GetResScale();
    ui_scale = { (int)res_scalar };
    Text::Info* mti = &menu_text.info;
    Text::Info* sti = &sup_text.info;

    //Menu and Supp text variables
    menu_text.SetOrigin();
    Vector2i m_t_pos = { 0 };
    string m_t_str = "MENU DEFAULT";

    sup_text.SetOrigin();
    Vector2i s_t_pos = { 0 };
    string s_t_str = "SUPPLEMENTARY DEFAULT";
    float s_t_str_max_w = -1;

    Sprite::Info elem_info = {}; elem_info.origin = ui_ori; elem_info.scale = ui_scale;
    int e_y_buffer = 0;
    //What we do depends on our label
    switch (label) {

        //Menus
        case Menus::CharCrea: {
            m_t_pos = Round(game->resolution.x * .5f, game->resolution.y * .1f); m_t_str = "Create Your Party";
            s_t_pos = Round(m_t_pos.x, game->resolution.y * .13f); s_t_str = "Use the options below to create your party of 4 adventurers";
            s_t_str_max_w = game->resolution.x * .33f;

            elem_info.pos = Round(game->resolution.x * .2f, game->resolution.y * .25f);
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };

            e_y_buffer = round(game->resolution.y * .1f);

            ui_elems.insert({ UIElem::Race_B, make_s<Button>(*this, elem_info, UIElem::Race_B)});
            sub_menus.insert({ Menus::CCRace, new Menu(Menus::CCRace)});

            elem_info.pos.y += e_y_buffer; //.35
            ui_elems.insert({ UIElem::BG_B, make_s<Button>(*this, elem_info, UIElem::BG_B) });
            sub_menus.insert({ Menus::CCBG, new Menu(Menus::CCBG)});

            elem_info.pos.y += e_y_buffer; //.45
            ui_elems.insert({ UIElem::Class_B, make_s<Button>(*this, elem_info, UIElem::Class_B) });
            sub_menus.insert({ Menus::CCClass, new Menu(Menus::CCClass)});

            elem_info.pos.y += e_y_buffer; //.55
            ui_elems.insert({ UIElem::AS, make_s<Button>(*this, elem_info, UIElem::AS) });
            sub_menus.insert({ Menus::CCAS, new Menu(Menus::CCAS)});

            elem_info.pos.y += e_y_buffer; //.65
            ui_elems.insert({ UIElem::Skills, make_s<Button>(*this, elem_info, UIElem::Skills) });
            sub_menus.insert({ Menus::CCSkills, new Menu(Menus::CCSkills)});

            elem_info.pos.y += e_y_buffer; //.75
            ui_elems.insert({ UIElem::Equipment_CC, make_s<Button>(*this, elem_info, UIElem::Equipment_CC) });
            sub_menus.insert({ Menus::CCEquip, new Menu(Menus::CCEquip)});


            ui_elems.insert({ UIElem::Create, make_s<Button>(*this, elem_info, UIElem::Create) });
            ui_elems.insert({ UIElem::Back, make_s<Button>(*this, elem_info, UIElem::Back) });

            break;
        }

        //Character Creation Sub-Menus
        case Menus::CCAS: {
            //Ability Scores: STR, CON, AGI, DEX, INT, WIS, CHA
            //Apparently need to decrease m_t & s_t font sizes (TO-DO)
            m_t_pos = Round(game->resolution.x * .5f, game->resolution.y * .2f); m_t_str = "Determine Your Ability Scores";
            s_t_pos = Round(m_t_pos.x, game->resolution.y * .23f); s_t_str = "Your Ability Scores --";
            s_t_str_max_w = game->resolution.x * .33f;

            elem_info.pos = Round(game->resolution.x * .5f, game->resolution.y * .2f);
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };

            e_y_buffer = round(game->resolution.y * .08f);

            ui_elems.insert({ UIElem::ASSTR, make_s<Picker>(*this, elem_info, UIElem::ASSTR) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::ASCON, make_s<Picker>(*this, elem_info, UIElem::ASCON) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::ASAGI, make_s<Picker>(*this, elem_info, UIElem::ASAGI) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::ASDEX, make_s<Picker>(*this, elem_info, UIElem::ASDEX) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::ASINT, make_s<Picker>(*this, elem_info, UIElem::ASINT) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::ASWIS, make_s<Picker>(*this, elem_info, UIElem::ASWIS) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::ASCHA, make_s<Picker>(*this, elem_info, UIElem::ASCHA) });

            break;
        }

        case Menus::CCBG: {
            //Backgrounds: Custom, Artist, Divine, Farmer, Innkeeper, Mechanic, Sailor, Soldier, Tradesman
            //Backgrounds give an ASI, Skills, and some equipment
            break;
        }

        case Menus::CCClass: {
            //Classes: Arcanist, Rogue, Warrior
            //Health / Skills / L1 features:
            //Arcanist: 6 / Arcana / Spells
            //Rogue: 8 / Thieving Tools / Expertise
            //Warrior: 10 / 2 PWS / Fighting Style
            //Ability Scores: STR, CON, AGI, DEX, INT, WIS, CHA
            //Adjust font sizes - TO-DO
            m_t_pos = Round(game->resolution.x * .5f, game->resolution.y * .2f); m_t_str = "Pick Your Class";
            s_t_pos = Round(m_t_pos.x, game->resolution.y * .23f); s_t_str = "Your Class determines your role in combat and the abilities you will learn as you progress";
            s_t_str_max_w = game->resolution.x * .33f;

            //Pickers
            elem_info.pos = Round(game->resolution.x * .5f, game->resolution.y * .2f);
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };

            e_y_buffer = round(game->resolution.y * .08f);

            ui_elems.insert({ UIElem::Class_P, make_s<Picker>(*this, elem_info, UIElem::Class_P) });
            break;
        }

        case Menus::CCEquip: {
            //Characters get 10 Aeons and 50 change, one weapon that they are proficient in, 3 rations, and a 
            // (plot-relevant?) token from their background
            break;
        }

        case Menus::CCRace: {
            //Races: Automaton (locks mechanical features & sex), Dwarf, Elf, Gnome, Human, Kobold
            //Race largely determines player aesthetics and influences several mechanics, including:
            // Size, Ability Score Adjustments, Sex, Skills, etc.
            //Elves also have a sub-race that limits their aesthetics
            //Aesthetics have suggested features, but none are locked out from any aesthetics (except Automata)
            //Adjust font size - TO-DO
            m_t_pos = Round(game->resolution.x * .5f, game->resolution.y * .2f); m_t_str = "Pick Your Race";
            s_t_pos = Round(m_t_pos.x, game->resolution.y * .23f); s_t_str = "Your Race influences your appearance, some of your capabilities, and how others react to you";
            s_t_str_max_w = game->resolution.x * .33f;

            //Pickers
            elem_info.pos = Round(game->resolution.x * .5f, game->resolution.y * .4f);
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };

            e_y_buffer = round(game->resolution.y * .1f);

            ui_elems.insert({ UIElem::Race_P, make_s<Picker>(*this, elem_info, UIElem::Race_P) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Size, make_s<Picker>(*this, elem_info, UIElem::Size) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Sex, make_s<Picker>(*this, elem_info, UIElem::Sex) });

            break;
        }

        case Menus::CCSkills: {
            //Action Skills (pickers)
            //Tool Skills (pickers)
            //Weapon skill(s) (toggles)
            break;
        }
        //End of CC Sub-Menus

        case Menus::Main: {

            m_t_pos = Round(game->resolution.x * .5f, game->resolution.y * .12f); m_t_str = m_t_str = "Seeds of Apocrypha";
            s_t_pos = Round(m_t_pos.x, game->resolution.y * .15f); s_t_str = "An Iron & Aether Adventure";

            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };
            elem_info.pos = Round(m_t_pos.x, game->resolution.y * .4f);

            e_y_buffer = round(game->resolution.y * .1f);

            ui_elems.insert({ UIElem::CharCrea, make_s<Button>(*this, elem_info, UIElem::CharCrea) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Load, make_s<Button>(*this, elem_info, UIElem::Load) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Tutorial, make_s<Button>(*this, elem_info, UIElem::Tutorial) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Options, make_s<Button>(*this, elem_info, UIElem::Options) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Quit, make_s<Button>(*this, elem_info, UIElem::Quit) });

            elem_info.pos = Round(game->resolution.x * .75f, game->resolution.y * .5f);
            ui_elems.insert({ UIElem::Debug_Room, make_s<Button>(*this, elem_info, UIElem::Debug_Room) });

            break;
        }

        case Menus::Options: {
            m_t_pos = Round(game->resolution.x * .5f, game->resolution.y * .12f); m_t_str = "Seeds of Apocrypha";
            s_t_pos = Round(m_t_pos.x, game->resolution.y * .15f); s_t_str = "Options";

            elem_info.pos = Round(m_t_pos.x, game->resolution.y * .3f);
            elem_info.sheet = "UI/Slider"; elem_info.frame_size = { 192, 27 };

            e_y_buffer = round(game->resolution.y * .09f);

            //Music and sfx sliders
            ui_elems.insert({ UIElem::Music_V, make_s<Slider>(*this, elem_info, UIElem::Music_V) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::SFX_V, make_s<Slider>(*this, elem_info, UIElem::SFX_V) });


            //Resolution picker, fullscreen toggle, and apply button
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };
            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Resolution, make_s<Picker>(*this, elem_info, UIElem::Resolution) });


            elem_info.sheet = "UI/Toggle"; elem_info.frame_size = { 24, 24 };
            elem_info.pos.y += e_y_buffer;
            //float f_t_x = elem_info.pos.x + game->resolution.x * (res_scalar * .01);
            ui_elems.insert({ UIElem::Fullscreen, make_s<Toggle>(*this, elem_info, UIElem::Fullscreen) });


            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };
            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Apply, make_s<Button>(*this, elem_info, UIElem::Apply) });


            //Back button
            elem_info.pos.y = round(game->resolution.y * .9f);
            ui_elems.insert({ UIElem::Back, make_s<Button>(*this, elem_info, UIElem::Back) });

            break;
        }
        
        case Menus::Options_G: {
            m_t_pos = Round(game->camera.GetCenter().x, game->camera.viewport.y + game->camera.viewport.h * .2f); m_t_str = "Options";
            s_t_pos = { 0 }; s_t_str = "";

            elem_info.pos = Round(m_t_pos.x, game->camera.viewport.y + game->camera.viewport.h * .28f);
            e_y_buffer = round(game->camera.viewport.y * .1f);
            elem_info.sheet = "UI/Slider"; elem_info.frame_size = { 192, 27 };

            //Music, sfx, and cam speed sliders
            ui_elems.insert({ UIElem::Music_V, make_s<Slider>(*this, elem_info, UIElem::Music_V) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::SFX_V, make_s<Slider>(*this, elem_info, UIElem::SFX_V) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::CAMSPD, make_s<Slider>(*this, elem_info, UIElem::CAMSPD) });

            //Edge Panning Toggle [TO-DO]

            //Buttons
            elem_info.sheet = "UI/Button"; elem_info.frame_size = { 93, 26 };

            //Close button
            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Close, make_s<Button>(*this, elem_info, UIElem::Close) });

            //Return to Title
            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Title, make_s<Button>(*this, elem_info, UIElem::Title) });

            //Quit
            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Quit, make_s<Button>(*this, elem_info, UIElem::Quit) });
            break;
        }
    }

    //Set our texts (not strictly necessary but keeping for now)
    menu_text.MoveTo(m_t_pos); mti->str = m_t_str; mti->max_width = game->resolution.x;
    sup_text.MoveTo(s_t_pos); sti->str = s_t_str; sup_text.SetOrigin({ .5f, .0f }); sti->max_width = game->resolution.x;
    if (s_t_str_max_w != -1)
        sti->max_width = s_t_str_max_w;

    //Add our UI elements to the scene entities vector
    for (const auto& ui : ui_elems)
        scene.AddEntity(ui.second);
}

void Menu::Update() {
    if (open) {
        Vector2i m_t_pos = { 0 };
        Vector2i s_t_pos = { 0 };
        Vector2i elem_pos = { 0 };
        int elem_y_buffer = 0.f;
        

        switch (label) {
            case Menus::Options_G:
                menu_text.info.pos = Round(game->camera.GetCenter().x, game->camera.viewport.y + game->camera.viewport.h * .2);

                elem_pos = Round(game->camera.GetCenter().x, game->camera.viewport.y + game->camera.viewport.h * .25f);

                //This would be easier to do with a vector, but switching from a u_map to a vector at this point would just be a pain in the ass
                for (const auto& ui : ui_elems) {
                    switch (ui.first) {
                        case UIElem::Music_V:
                            elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .25f);
                        break;

                        case UIElem::SFX_V:
                            elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .35f);
                        break;

                        case UIElem::CAMSPD:
                            elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .45f);
                        break;

                        case UIElem::Close:
                            elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .55f);
                        break;

                        case UIElem::Title:
                            elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .65f);
                        break;

                        case UIElem::Quit:
                            elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .75f);
                        break;
                    }
                    ui.second->MoveTo(elem_pos);
                }
            break;
        }

        for (const auto& s_m : sub_menus)
            s_m.second->Update();
    }

}

void Menu::Draw() {
    if (open) {
        game->renderer.DrawTxt(menu_text);
        game->renderer.DrawTxt(sup_text);

        for (auto& sm : sub_menus)
            sm.second->Draw();
    }
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
    int res_scalar = game->GetResScale();

    //A more efficient way to do this would be to base the new size on the old size and do some funky math shit with the scalar, but this works for now
    Vector2i new_m_t_pos = Round(game->resolution.x * .5f, game->resolution.y * .5f);
    Vector2i new_s_t_pos = new_m_t_pos;
    switch (label) {
    case Menus::CharCrea:
        new_m_t_pos.y = round(game->resolution.y * .1f);
        new_s_t_pos.y = round(game->resolution.y * .13f);
        break;

    case Menus::CCAS:
    case Menus::CCBG:
    case Menus::CCClass:
    case Menus::CCEquip:
    case Menus::CCRace:
    case Menus::CCSkills:
        new_m_t_pos.y = round(game->resolution.y * .2f);
        new_s_t_pos.y = round(game->resolution.y * .23f);
        break;

    case Menus::Main:
    case Menus::Options:
        new_m_t_pos.y = round(game->resolution.y * .15f);
        new_s_t_pos.y = round(game->resolution.y * .2f);
        break;
    }

    menu_text.SetOrigin();
    menu_text.info.pos = new_m_t_pos;
    sup_text.SetOrigin({ .5f, .0f });
    sup_text.info.pos = new_s_t_pos;

    //UI Elements
    Vector2i new_pos = Round(game->resolution * .5f);

    for (const auto& ui : ui_elems) {
        auto& u = ui.second;

        u->Resize(res_scalar);

        //Adjust the positions - there has to be a more efficient goddamn way of doing this
        Vector2u gr = game->resolution;
        switch (ui.first) {
            case UIElem::Apply:
                new_pos = Round(gr.x * .5f, gr.y * .66f);
                break;

            case UIElem::AS:
                new_pos = Round(gr.x * .2f, gr.y * .55f);
                break;

            case UIElem::ASAGI:
                new_pos = Round(gr.x * .5f, gr.y * .36f);
                break;

            case UIElem::ASCHA:
                new_pos = Round(gr.x * .5f, gr.y * .68f);
                break;

            case UIElem::ASCON:
                new_pos = Round(gr.x * .5f, gr.y * .28f);
                break;

            case UIElem::ASDEX:
                new_pos = Round(gr.x * .5f, gr.y * .44f);
                break;

            case UIElem::ASINT:
                new_pos = Round(gr.x * .5f, gr.y * .52f);
                break;

            case UIElem::ASSTR:
                new_pos = Round(gr.x * .5f, gr.y * .2f);
                break;

            case UIElem::ASWIS:
                new_pos = Round(gr.x * .5f, gr.y * .6f);
                break;

            case UIElem::Back:
                new_pos = Round(gr.x * .5f, gr.y * .9f);
                break;

            case UIElem::BG_B:
                new_pos = Round(gr.x * .2f, gr.y * .35f);
                break;

            case UIElem::CharCrea:
                new_pos = Round(gr.x * .5f, gr.y * .4f);
                break;

            case UIElem::Class_B:
                new_pos = Round(gr.x * .2f, gr.y * .45f);
                break;

            case UIElem::Create:
                new_pos = Round(gr.x * .5f, gr.y * .8f);
                break;

            case UIElem::Debug_Room:
                new_pos = Round(gr.x * .75f, gr.y * .5f);
                break;

            case UIElem::Equipment_CC:
                new_pos = Round(gr.x * .2f, gr.y * .75f);
                break;

            case UIElem::Fullscreen: {
                new_pos = Round(gr.x * (.5f + res_scalar * .01f), gr.y * .57f );
                string new_status = "False";
                if (game->resolution == game->window.ScreenSize())
                    new_status = "True";

                SetUIElemStatus(ui.first, new_status);
            } break;

            case UIElem::Load:
                new_pos = Round(gr.x * .5f, gr.y * .5f);
                break;

            case UIElem::Music_V:
                new_pos = Round(gr.x * .5f, gr.y * .3f);
                break;

            case UIElem::Options:
                new_pos = Round(gr.x * .5f, gr.y * .7f);
                break;

            case UIElem::Quit:
                new_pos = Round(gr.x * .5f, gr.y * .8f);
                break;

            case UIElem::Race_B:
                new_pos = Round(gr.x * .2f, gr.y * .25f);
                break;

            case UIElem::Resolution:
                new_pos = Round(gr.x * .5f, gr.y * .48f);
                break;

            case UIElem::SFX_V:
                new_pos = Round(gr.x * .5f, gr.y * .39f);
                break;

            case UIElem::Skills:
                new_pos = Round(gr.x * .2f, gr.y * .65f);
                break;

            case UIElem::Tutorial:
                new_pos = Round(gr.x * .5f, gr.y * .6f);
                break;
        }

        u->MoveTo(new_pos);
    }
}

void Menu::AddUIElem(const s_ptr<UI> new_elem) {
    ui_elems.insert({ new_elem.get()->GetElem(), new_elem });
    scene.AddEntity(new_elem);
}

void Menu::RemoveUIElem(const UIElem ui) {
    if (CheckUIElem(ui)) {
        ui_elems[ui].get()->alive = false;
        ui_elems[ui].reset();
        ui_elems.erase(ui);
    }
}

bool Menu::CheckUIElem(const UIElem ui) {
    if (ui_elems.find(ui) != ui_elems.end()) return true;

    cout << "That UI Element does not exist!" << endl;
    return false;
}

void Menu::SetUIElemStatus(const UIElem ui, const string new_status) {

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

void Menu::SetUIElemActive(const UIElem ui, const bool a) {
    if (CheckUIElem(ui))
        ui_elems[ui]->SetActive(a);
}

string Menu::GetUIElemStatus(const UIElem ui) {

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

bool Menu::GetUIElemActive(const UIElem ui) {
    if (CheckUIElem(ui))
        return ui_elems[ui]->GetActive();

    return false;
}
