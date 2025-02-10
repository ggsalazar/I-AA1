#include "Button.h"

void Button::Draw(const bool debug) {
    if (active and Selected(MOUSEPOS))
        structure.window.draw(bbox_debug);

    Entity::Draw(debug);
    structure.window.draw(label);
}

void Button::Move() {
    Entity::Move();
    label_offset = structure.game.GetResScale();
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
                    structure.game.SetResolution(sf::Vector2u(SCREENW(), SCREENH()));
                else {
                    uint new_scale = stoi(menu.GetUIElemStatus(UIElems::RESOLUTION));
                    uint old_scale = structure.game.GetResScale();
                    if (new_scale != old_scale)
                        structure.game.SetResolution(new_scale);
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
            structure.scene->OpenMenu(Menus::MAIN);
        break;

        case UIElems::CHARCREA:
            menu.Open(false);
            
            structure.scene->CreatePartyMember();
            structure.scene->OpenMenu(Menus::CHARCREA);
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
            structure.scene->OpenMenu(Menus::OPTIONS);
        break;

        case UIElems::MAINMENU:
            menu.Open(false);
            structure.game.paused = false;
            structure.game.game_over = false;
            structure.game.title_scene->AddEntity(shared_from_this());
            structure.game.SetScene(Scenes::TITLE);
        break;

        case UIElems::QUIT:
            structure.window.close();
        break;

        case UIElems::RESUME:
            menu.Open(false);
            structure.game.paused = false;
        break;

        case UIElems::TUTORIAL:
            structure.scene->CreatePreGen(PreGens::DAKN);
            structure.game.area = Areas::TUTTON;
            structure.game.SetScene(Scenes::AREA);
        break;
    }
}
