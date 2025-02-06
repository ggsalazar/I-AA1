#include "Picker.h"

Picker::Picker(const Structure& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style, const int init_dfc) :
    UI(s, m, a_i, t, style, init_dfc), picking(structure.game.default_font) {

    label_offset = structure.game.GetResScale()*8;
    label.setPosition({ pos.x, pos.y - label_offset });

    //Set up position.x bbox
    l_bbox.position.x = bbox.position.x + bbox.size.x * .05;
    l_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    l_bbox.size.x = bbox.size.x*.25;
    l_bbox.size.y = bbox.size.y*.75;
    //Set position.x bbox debug
    l_bbox_debug.setPosition(sf::Vector2f(l_bbox.position.x, l_bbox.position.y));
    l_bbox_debug.setSize(sf::Vector2f(l_bbox.size.x, l_bbox.size.y));
    l_bbox_debug.setFillColor(sf::Color(255, 0, 0, 127)); //Red, 50% opacity

    //Right bbox
    r_bbox.position.x = bbox.position.x + bbox.size.x*.7;
    r_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    r_bbox.size.x = bbox.size.x * .25;
    r_bbox.size.y = bbox.size.y * .75;
    //Right bbox debug
    r_bbox_debug.setPosition(sf::Vector2f(r_bbox.position.x, r_bbox.position.y));
    r_bbox_debug.setSize(sf::Vector2f(r_bbox.size.x, r_bbox.size.y));
    r_bbox_debug.setFillColor(sf::Color(0, 0, 255, 127)); //Blue, 50% opacity

    //What exactly ARE we picking?
    picking = label;
    string picking_str = "";
    switch (elem) {
        case UIElems::BACKGROUND:
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

        case UIElems::CLASS:
            options.push_back("Arcanist");
            options.push_back("Rogue");
            options.push_back("Warrior");
            picking_str = "Warrior";
            option_picked = options.end()-1;
        break;

        case UIElems::RACE:
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
            picking_str = to_string((int)structure.game.GetResScale());
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
    picking.setPosition(pos);
}

void Picker::GetInput() {
    if (active) {
        if (LeftSelected(MOUSEPOS)) {
            if (Input::KeyPressed(LMB))
                LeftPressed();
            if (Input::KeyReleased(LMB) and l_primed)
                LeftReleased();
        }
        else if (!LeftSelected(MOUSEPOS)) l_primed = false;
        
        if (RightSelected(MOUSEPOS)) {
            if (Input::KeyPressed(LMB))
                RightPressed();
            if (Input::KeyReleased(LMB) and r_primed)
                RightReleased();
        }
        else if (!RightSelected(MOUSEPOS)) r_primed = false;
    }
}

void Picker::Draw(const bool debug) {
    if (active and Selected(MOUSEPOS))
        structure.window.draw(bbox_debug);

    Entity::Draw(debug);
    structure.window.draw(label);

    structure.window.draw(picking);

    if (active) {
        if (LeftSelected(MOUSEPOS))
            structure.window.draw(l_bbox_debug);
        else if (RightSelected(MOUSEPOS))
            structure.window.draw(r_bbox_debug);

        if (elem == UIElems::SEX and picking.getString() == "-") {
            string new_sex = rand() % 2 ? "Male" : "Female";
            Text::SetStr(picking, new_sex);
        }
    }
}

void Picker::Move() {
    Entity::Move();
    label_offset = structure.game.GetResScale() * 5;
    label.setPosition({ pos.x, pos.y - label_offset });

    //Move position.x bbox+debug
    l_bbox.position.x = bbox.position.x + bbox.size.x * .05;
    l_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    l_bbox.size.x = bbox.size.x * .25;
    l_bbox.size.y = bbox.size.y * .75;

    l_bbox_debug.setPosition(sf::Vector2f(l_bbox.position.x, l_bbox.position.y));
    l_bbox_debug.setSize(sf::Vector2f(l_bbox.size.x, l_bbox.size.y));
    l_bbox_debug.setFillColor(sf::Color(255, 0, 0, 127)); //Red, 50% opacity

    //Right bbox+debug
    r_bbox.position.x = bbox.position.x + bbox.size.x * .7;
    r_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    r_bbox.size.x = bbox.size.x * .25;
    r_bbox.size.y = bbox.size.y * .75;

    r_bbox_debug.setPosition(sf::Vector2f(r_bbox.position.x, r_bbox.position.y));
    r_bbox_debug.setSize(sf::Vector2f(r_bbox.size.x, r_bbox.size.y));
    r_bbox_debug.setFillColor(sf::Color(0, 0, 255, 127)); //Blue, 50% opacity

    picking.setPosition(pos);
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
        case UIElems::BACKGROUND:
        case UIElems::CLASS:
        case UIElems::RACE:
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
                curr_res = floor(SCREENW() / MINRESW);

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
            string race = menu.GetUIElemStatus(UIElems::RACE);

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
            //Gnomes and Kobolds can only be Small (Set in RACE - TO-DO)
            p = *option_picked;
        break;
    }

    Text::SetStr(picking, p);
}

void Picker::RightReleased() {
    string p = picking.getString();
    switch (elem) {
        case UIElems::BACKGROUND:
        case UIElems::CLASS:
        case UIElems::RACE:
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

            if (++curr_res > floor(SCREENW() / MINRESW))
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
            string race = menu.GetUIElemStatus(UIElems::RACE);

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
            //Gnomes and Kobolds can only be Small (Set in RACE - TO-DO)
            p = *option_picked;
            break;
    }

    Text::SetStr(picking, p);
}
