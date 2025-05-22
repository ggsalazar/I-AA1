#include "Picker.h"

Picker::Picker(Menu& m, const Sprite::Info& s_i, const UIElem e,
    const uint init_ui_layer)
    : UI(m, s_i, e, init_ui_layer), picking(&game->default_fonts[36]) {

    label_offset = game->GetResScale() * 6;
    label.MoveTo({ pos.x, pos.y - label_offset });

    //Set up bbox
    l_bbox.x = bbox.x + bbox.w * .05;
    l_bbox.y = bbox.y + bbox.h * .1;
    l_bbox.w = bbox.w * .25;
    l_bbox.h = bbox.h * .75;

    //Right bbox
    r_bbox.x = bbox.x + bbox.w * .7;
    r_bbox.y = bbox.y + bbox.h * .1;
    r_bbox.w = bbox.w * .25;
    r_bbox.h = bbox.h * .75;

    //What exactly ARE we picking?
    string picking_str = "";
    switch (elem) {
    case UIElem::BG_P:
        options.push_back("Custom");
        options.push_back("Artist");
        options.push_back("Divine");
        options.push_back("Farmer");
        options.push_back("Innkeeper");
        options.push_back("Mechanic");
        options.push_back("Sailor");
        options.push_back("Soldier");
        options.push_back("Tradesman");
        picking_str = "Tradesman";
        option_picked = options.end() - 1;
        break;

    case UIElem::Class_P:
        options.push_back("Arcanist");
        options.push_back("Rogue");
        options.push_back("Warrior");
        picking_str = "Warrior";
        option_picked = options.end() - 1;
        break;

    case UIElem::Race_P:
        options.push_back("Automaton");
        options.push_back("Dwarf");
        options.push_back("Elf");
        options.push_back("Gnome");
        options.push_back("Human");
        options.push_back("Kobold");
        picking_str = "Human";
        option_picked = options.end() - 2;
        break;

    case UIElem::Resolution:
        picking_str = to_string(game->GetResScale());
        break;

    case UIElem::Sex:
        picking_str = rand() % 2 ? "Male" : "Female";
        break;

    case UIElem::Size:
        options.push_back("Small");
        options.push_back("Medium");
        options.push_back("Big");
        picking_str = "Medium";
        option_picked = options.begin() + 1;
        break;
    }
    picking.info.str = picking_str;
    picking.SetOrigin();
    picking.MoveTo({ pos.x, pos.y + 2 });
}

void Picker::GetInput() {
    if (active) {
        if (LeftSelected()) {
            if (Input::BtnPressed(LMB))
                LeftPressed();
            if (Input::BtnReleased(LMB) and l_primed)
                LeftReleased();
        }
        else if (!LeftSelected()) l_primed = false;

        if (RightSelected()) {
            if (Input::BtnPressed(LMB))
                RightPressed();
            if (Input::BtnReleased(LMB) and r_primed)
                RightReleased();
        }
        else if (!RightSelected()) r_primed = false;
    }
}

void Picker::Draw() {
    if (active and Selected())
        game->renderer.DrawRect(bbox, Color(0, 1, 0));

    Entity::Draw();

    game->renderer.DrawTxt(label);
    game->renderer.DrawTxt(picking);

    if (active) {
        if (LeftSelected())
            game->renderer.DrawRect(l_bbox, Color(1, 0, 0, 0), Color(1, 0, 0, .5)); //Red, 50% opacity
        else if (RightSelected())
            game->renderer.DrawRect(r_bbox, Color(0, 0, 1, 0), Color(0, 0, 1, .5)); //Blue, 50% opacity

        if (elem == UIElem::Sex and picking.info.str == "-") {
            string new_sex = rand() % 2 ? "Male" : "Female";
            picking.info.str = new_sex;
        }
    }
}

void Picker::Move() {
    //Move debug and bbox
    pos_debug.x = pos.x;
    pos_debug.y = pos.y;

    size = sprite.GetSprSize();

    //bbox position will always be top left
    bbox.x = pos.x - sprite.GetOrigin().x * size.x;
    bbox.y = pos.y - sprite.GetOrigin().y * size.y;
    bbox.w = size.x;
    bbox.h = size.y;

    //Move bboxes
    l_bbox.x = bbox.x + bbox.w * .05;
    l_bbox.y = bbox.y + bbox.h * .1;
    l_bbox.w = bbox.w * .25;
    l_bbox.h = bbox.h * .75;

    r_bbox.x = bbox.x + bbox.w * .7;
    r_bbox.y = l_bbox.y;
    r_bbox.w = l_bbox.w;
    r_bbox.h = l_bbox.h;


    //Move everything else
    label_offset = game->GetResScale() * 6;
    label.MoveTo({ pos.x, pos.y - label_offset });

    picking.MoveTo(pos);
}

void Picker::SetPicking(const string new_p) {
    picking.info.str = new_p;
    if (elem == UIElem::Size) {
        if (new_p == "Small") option_picked = options.begin();
        else if (new_p == "Medium") option_picked = options.begin() + 1;
        else option_picked = options.end() - 1;
    }
}

void Picker::LeftReleased() {
    string p = picking.info.str;
    switch (elem) {
    case UIElem::BG_P:
    case UIElem::Class_P:
    case UIElem::Race_P:
        if (option_picked == options.begin())
            option_picked = options.end() - 1;
        else --option_picked;

        p = *option_picked;

        //Automata do not have a sex
        if (p == "Automaton") {
            menu.SetUIElemActive(UIElem::Sex, false);
            menu.SetUIElemStatus(UIElem::Sex, "-");
        }
        else
            menu.SetUIElemActive(UIElem::Sex);

        //Dwarves can be Small or Med
        if (p == "Dwarf") {
            menu.SetUIElemActive(UIElem::Size);
            if (menu.GetUIElemStatus(UIElem::Size) == "Big")
                menu.SetUIElemStatus(UIElem::Size, "Medium");
        }
        //Elves can be Med or Big
        else if (p == "Elf") {
            menu.SetUIElemActive(UIElem::Size);
            if (menu.GetUIElemStatus(UIElem::Size) == "Small")
                menu.SetUIElemStatus(UIElem::Size, "Medium");
        }
        //Gnomes and Kobolds can only be Small
        else if (p == "Gnome" or p == "Kobold") {
            menu.SetUIElemActive(UIElem::Size, false);
            menu.SetUIElemStatus(UIElem::Size, "Small");
        }
        else menu.SetUIElemActive(UIElem::Size);
        break;

    case UIElem::Resolution: {
        uint curr_res = stoi(p);

        if (--curr_res < 1)
            curr_res = floor(game->window.ScreenSize().x / game->GetMinRes().x);

        p = to_string(curr_res);

        //Set the Apply button to active
        menu.SetUIElemActive(UIElem::Apply);
        break;
    }

    case UIElem::Sex:
        p = p == "Male" ? "Female" : "Male";
        break;


    case UIElem::Size:
        //What Size you can be depends on your race
        string race = menu.GetUIElemStatus(UIElem::Race_P);

        //Automata and Humans can be S/M/B
        if (race == "Automaton" or race == "Human") {
            if (option_picked == options.begin())
                option_picked = options.end() - 1;
            else --option_picked;
        }
        //Dwarves can be Small or Med
        else if (race == "Dwarf") {
            if (option_picked == options.begin())
                option_picked = options.end() - 2;
            else --option_picked;
        }
        //Elves can be Med or Big
        else if (race == "Elf") {
            if (option_picked == options.begin() + 1)
                option_picked = options.end() - 1;
            else --option_picked;
        }
        //Gnomes and Kobolds can only be Small (Set in RACE)
        p = *option_picked;
        break;
    }

    picking.info.str = p;
}

void Picker::RightReleased() {
    string p = picking.info.str;
    switch (elem) {
    case UIElem::BG_P:
    case UIElem::Class_P:
    case UIElem::Race_P:
        if (option_picked == options.end() - 1)
            option_picked = options.begin();
        else ++option_picked;

        p = *option_picked;

        //Automata do not have a sex
        if (p == "Automaton") {
            menu.SetUIElemActive(UIElem::Sex, false);
            menu.SetUIElemStatus(UIElem::Sex, "-");
        }
        else
            menu.SetUIElemActive(UIElem::Sex);

        //Dwarves can be Small or Med
        if (p == "Dwarf") {
            menu.SetUIElemActive(UIElem::Size);
            if (menu.GetUIElemStatus(UIElem::Size) == "Big")
                menu.SetUIElemStatus(UIElem::Size, "Medium");
        }
        //Elves can be Med or Big
        else if (p == "Elf") {
            menu.SetUIElemActive(UIElem::Size);
            if (menu.GetUIElemStatus(UIElem::Size) == "Small")
                menu.SetUIElemStatus(UIElem::Size, "Medium");
        }
        //Gnomes and Kobolds can only be Small
        else if (p == "Gnome" or p == "Kobold") {
            menu.SetUIElemActive(UIElem::Size, false);
            menu.SetUIElemStatus(UIElem::Size, "Small");
        }
        else menu.SetUIElemActive(UIElem::Size);
        break;

    case UIElem::Resolution: {
        uint curr_res = stoi(p);

        if (++curr_res > floor(game->window.ScreenSize().x / game->GetMinRes().x))
            curr_res = 1;

        p = to_string(curr_res);

        //Set the Apply button to active
        menu.SetUIElemActive(UIElem::Apply);
        break;
    }

    case UIElem::Sex:
        p = p == "Male" ? "Female" : "Male";
        break;


    case UIElem::Size:
        //What Size you can be depends on your race
        string race = menu.GetUIElemStatus(UIElem::Race_P);

        //Automata and Humans can be S/M/B
        if (race == "Automaton" or race == "Human") {
            if (option_picked == options.end() - 1)
                option_picked = options.begin();
            else ++option_picked;
        }
        //Dwarves can be Small or Med
        else if (race == "Dwarf") {
            if (option_picked == options.end() - 2)
                option_picked = options.begin();
            else ++option_picked;
        }
        //Elves can be Med or Big
        else if (race == "Elf") {
            if (option_picked == options.end() - 1)
                option_picked = options.end() - 2;
            else ++option_picked;
        }
        //Gnomes and Kobolds can only be Small (Set in RACE)
        p = *option_picked;
        break;
    }

    picking.info.str = p;
}
