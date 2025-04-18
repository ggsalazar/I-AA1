#include "Picker.h"

Picker::Picker(const Engine& e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style,
    const uint init_ui_layer, const int init_dfc) :
    UI(e, m, a_i, t, style, init_ui_layer, init_dfc), picking(engine.game.default_font) {

    label_offset = engine.game.GetResScale()*8;
    label.setPosition(Vector2f(pos.x, pos.y - label_offset));

    //Set up position.x bbox
    l_bbox.position.x = bbox.position.x + bbox.size.x * .05;
    l_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    l_bbox.size.x = bbox.size.x*.25;
    l_bbox.size.y = bbox.size.y*.75;
    //Set position.x bbox debug
    l_bbox_debug.setPosition(Vector2f(l_bbox.position.x, l_bbox.position.y));
    l_bbox_debug.setSize(Vector2f(l_bbox.size.x, l_bbox.size.y));
    l_bbox_debug.setFillColor(sf::Color(255, 0, 0, 127)); //Red, 50% opacity

    //Right bbox
    r_bbox.position.x = bbox.position.x + bbox.size.x*.7;
    r_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    r_bbox.size.x = bbox.size.x * .25;
    r_bbox.size.y = bbox.size.y * .75;
    //Right bbox debug
    r_bbox_debug.setPosition(Vector2f(r_bbox.position.x, r_bbox.position.y));
    r_bbox_debug.setSize(Vector2f(r_bbox.size.x, r_bbox.size.y));
    r_bbox_debug.setFillColor(sf::Color(0, 0, 255, 127)); //Blue, 50% opacity

    //What exactly ARE we picking?
    picking = label;
    string picking_str = "";
    switch (elem) {
        case UIElems::BACKGROUND_P:
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
            option_picked = options.end()-1;
        break;

        case UIElems::CLASS_P:
            options.push_back("Arcanist");
            options.push_back("Rogue");
            options.push_back("Warrior");
            picking_str = "Warrior";
            option_picked = options.end()-1;
        break;

        case UIElems::RACE_P:
            options.push_back("Automaton");
            options.push_back("Dwarf");
            options.push_back("Elf");
            options.push_back("Gnome");
            options.push_back("Human");
            options.push_back("Kobold");
            picking_str = "Human";
            option_picked = options.end()-2;
        break;

        case UIElems::RESOLUTION:
            picking_str = to_string((int)engine.game.GetResScale());
        break;

        case UIElems::SEX:
            picking_str = rand()%2 ? "Male" : "Female";
        break;

        case UIElems::SIZE:
            options.push_back("Small");
            options.push_back("Medium");
            options.push_back("Big");
            picking_str = "Medium";
            option_picked = options.begin()+1;
        break;
    }
    Text::SetStr(picking, picking_str);
    picking.setPosition(Vector2f(pos));
}

void Picker::GetInput() {
    if (active) {
        if (LeftSelected(MOUSEPOS_E)) {
            if (Input::KeyPressed(LMB))
                LeftPressed();
            if (Input::KeyReleased(LMB) and l_primed)
                LeftReleased();
        }
        else if (!LeftSelected(MOUSEPOS_E)) l_primed = false;
        
        if (RightSelected(MOUSEPOS_E)) {
            if (Input::KeyPressed(LMB))
                RightPressed();
            if (Input::KeyReleased(LMB) and r_primed)
                RightReleased();
        }
        else if (!RightSelected(MOUSEPOS_E)) r_primed = false;
    }
}

void Picker::Draw() {
    if (active and Selected(MOUSEPOS_E))
        engine.window.draw(bbox_debug);

    Entity::Draw();
    engine.window.draw(label);

    engine.window.draw(picking);

    if (active) {
        if (LeftSelected(MOUSEPOS_E))
            engine.window.draw(l_bbox_debug);
        else if (RightSelected(MOUSEPOS_E))
            engine.window.draw(r_bbox_debug);

        if (elem == UIElems::SEX and picking.getString() == "-") {
            string new_sex = rand() % 2 ? "Male" : "Female";
            Text::SetStr(picking, new_sex);
        }
    }
}

void Picker::Move() {
    label_offset = engine.game.GetResScale() * 5;
    label.setPosition(Vector2f(pos.x, pos.y - label_offset));

    //Move position.x bbox+debug
    l_bbox.position.x = bbox.position.x + bbox.size.x * .05;
    l_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    l_bbox.size.x = bbox.size.x * .25;
    l_bbox.size.y = bbox.size.y * .75;

    l_bbox_debug.setPosition(l_bbox.position);
    l_bbox_debug.setSize(l_bbox.size);
    l_bbox_debug.setFillColor(sf::Color(255, 0, 0, 127)); //Red, 50% opacity

    //Right bbox+debug
    r_bbox.position.x = bbox.position.x + bbox.size.x * .7;
    r_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    r_bbox.size.x = bbox.size.x * .25;
    r_bbox.size.y = bbox.size.y * .75;

    r_bbox_debug.setPosition(r_bbox.position);
    r_bbox_debug.setSize(r_bbox.size);
    r_bbox_debug.setFillColor(sf::Color(0, 0, 255, 127)); //Blue, 50% opacity

    Text::SetCharSize(picking, engine.game.GetResScale() * 12);
    picking.setPosition(Vector2f(pos));
}

void Picker::SetPicking(const string new_p) {
    Text::SetStr(picking, new_p);
    if (elem == UIElems::SIZE) {
        if (new_p == "Small") option_picked = options.begin();
        else if (new_p == "Medium") option_picked = options.begin() + 1;
        else option_picked = options.end() - 1;
    }
}

void Picker::LeftReleased() {
    string p = picking.getString();
    switch (elem) {
        case UIElems::BACKGROUND_P:
        case UIElems::CLASS_P:
        case UIElems::RACE_P:
            if (option_picked == options.begin())
                option_picked = options.end()-1;
            else --option_picked;

            p = *option_picked;

            //Automata do not have a sex
            if (p == "Automaton") {
                menu.SetUIElemActive(UIElems::SEX, false);
                menu.SetUIElemStatus(UIElems::SEX, "-");
            }
            else
                menu.SetUIElemActive(UIElems::SEX);

            //Dwarves can be Small or Med
            if (p == "Dwarf") {
                menu.SetUIElemActive(UIElems::SIZE);
                if (menu.GetUIElemStatus(UIElems::SIZE) == "Big")
                    menu.SetUIElemStatus(UIElems::SIZE, "Medium");
            }
            //Elves can be Med or Big
            else if (p == "Elf") {
                menu.SetUIElemActive(UIElems::SIZE);
                if (menu.GetUIElemStatus(UIElems::SIZE) == "Small")
                    menu.SetUIElemStatus(UIElems::SIZE, "Medium");
            }
            //Gnomes and Kobolds can only be Small
            else if (p == "Gnome" or p == "Kobold") {
                menu.SetUIElemActive(UIElems::SIZE, false);
                menu.SetUIElemStatus(UIElems::SIZE, "Small");
            }
            else menu.SetUIElemActive(UIElems::SIZE);
        break;

        case UIElems::RESOLUTION: {
            uint curr_res = stoi(p);

            if (--curr_res < 1)
                curr_res = floor(Display::ScreenSize().x / Display::MinRes().x);

            p = to_string(curr_res);

            //Set the Apply button to active
            menu.SetUIElemActive(UIElems::APPLY);
            break;
        }

        case UIElems::SEX:
            p = p == "Male" ? "Female" : "Male";
        break;


        case UIElems::SIZE:
            //What Size you can be depends on your race
            string race = menu.GetUIElemStatus(UIElems::RACE_P);

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
                if (option_picked == options.begin()+1)
                    option_picked = options.end() - 1;
                else --option_picked;
            }
            //Gnomes and Kobolds can only be Small (Set in RACE)
            p = *option_picked;
        break;
    }

    Text::SetStr(picking, p);
}

void Picker::RightReleased() {
    string p = picking.getString();
    switch (elem) {
        case UIElems::BACKGROUND_P:
        case UIElems::CLASS_P:
        case UIElems::RACE_P:
            if (option_picked == options.end()-1)
                option_picked = options.begin();
            else ++option_picked;

            p = *option_picked;

            //Automata do not have a sex
            if (p == "Automaton") {
                menu.SetUIElemActive(UIElems::SEX, false);
                menu.SetUIElemStatus(UIElems::SEX, "-");
            }
            else 
                menu.SetUIElemActive(UIElems::SEX);

            //Dwarves can be Small or Med
            if (p == "Dwarf") {
                menu.SetUIElemActive(UIElems::SIZE);
                if (menu.GetUIElemStatus(UIElems::SIZE) == "Big")
                    menu.SetUIElemStatus(UIElems::SIZE, "Medium");
            }
            //Elves can be Med or Big
            else if (p == "Elf") {
                menu.SetUIElemActive(UIElems::SIZE);
                if (menu.GetUIElemStatus(UIElems::SIZE) == "Small")
                    menu.SetUIElemStatus(UIElems::SIZE, "Medium");
            }
            //Gnomes and Kobolds can only be Small
            else if (p == "Gnome" or p == "Kobold") {
                menu.SetUIElemActive(UIElems::SIZE, false);
                menu.SetUIElemStatus(UIElems::SIZE, "Small");
            }
            else menu.SetUIElemActive(UIElems::SIZE);
        break;

        case UIElems::RESOLUTION: {
            uint curr_res = stoi(p);

            if (++curr_res > floor(Display::ScreenSize().x / Display::MinRes().x))
                curr_res = 1;

            p = to_string(curr_res);

            //Set the Apply button to active
            menu.SetUIElemActive(UIElems::APPLY);
            break;
        }

        case UIElems::SEX:
            p = p == "Male" ? "Female" : "Male";
        break;


        case UIElems::SIZE:
            //What Size you can be depends on your race
            string race = menu.GetUIElemStatus(UIElems::RACE_P);

            //Automata and Humans can be S/M/B
            if (race == "Automaton" or race == "Human") {
                if (option_picked == options.end()-1)
                    option_picked = options.begin();
                else ++option_picked;
            }
            //Dwarves can be Small or Med
            else if (race == "Dwarf") {
                if (option_picked == options.end()-2)
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

    Text::SetStr(picking, p);
}
