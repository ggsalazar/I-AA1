#include "Button.h"

void Button::Draw() {
    if (active and Selected(sf::Vector2i(MOUSEPOS_W_E)))
        engine.window.draw(bbox_debug);

    Entity::Draw();
    engine.window.draw(label);
}

void Button::Move(sf::Vector2f offset) {
    Entity::Move(offset);
    label_offset = engine.game.GetResScale();
    label.setPosition({ pos.x, pos.y - label_offset });
}

void Button::MoveTo(sf::Vector2f new_pos) {
    Entity::MoveTo(new_pos);
    label_offset = engine.game.GetResScale();
    label.setPosition({ pos.x, pos.y - label_offset });
}

void Button::Pressed() {
    UI::Pressed();
}

void Button::Released() {
    activated = true;
    switch (elem) {
        case UIElems::APPLY:
            if (menu.label == Menus::OPTIONS) {
                //Set the game's current resolution to the scale determined by the resolution picker OR set it to fullscreen if that toggle is clicked
                if (menu.GetUIElemStatus(UIElems::FULLSCREEN) == "True")
                    engine.game.SetResolution(SCREENSIZE());
                else {
                    uint new_scale = stoi(menu.GetUIElemStatus(UIElems::RESOLUTION));
                    uint old_scale = engine.game.GetResScale();
                    if (new_scale != old_scale)
                        engine.game.SetResolution(new_scale);
                }
                SetActive(false);
            }
        break;

        case UIElems::AS:
            //Close whatever other sub-menu is open - TO-DO
            //Open Ability Score sub-menu
        break;
        
        case UIElems::BACK:
            //If creating a character, ask to confirm and if yes, wipe party creation
            menu.Open(false);
            engine.scene->OpenMenu(Menus::MAIN);
        break;

        case UIElems::CHARCREA:
            menu.Open(false);
            
            engine.scene->CreatePartyMem();
            engine.scene->OpenMenu(Menus::CHARCREA);
        break;

        case UIElems::CLOSE:
            menu.Open(false);
        break;

        case UIElems::CREATE: {
            //Spawn Name entry box and "Finish/OK" button
            break;
        }
        
        case UIElems::RACE_B:
            //Close whatever other sub-menu is open - TO-DO
            //Open Race sub-menu
            menu.OpenSM(Menus::CCRACE);
        break;

        case UIElems::OPTIONS:
            menu.Open(false);
            engine.scene->OpenMenu(Menus::OPTIONS);
        break;

        case UIElems::QUIT:
            engine.window.close();
        break;

        case UIElems::RESUME:
            menu.Open(false);
            engine.game.paused = false;
        break;

        case UIElems::TITLE:
            engine.game.paused = false;
            engine.game.game_over = false;
            engine.game.SetScene(Scenes::TITLE);
        break;

        case UIElems::TUTORIAL:
            engine.scene->CreatePreGen(PreGens::DAKN);
            engine.game.area = Areas::TUTTON;
            engine.game.SetScene(Scenes::AREA);
        break;
    }
}
