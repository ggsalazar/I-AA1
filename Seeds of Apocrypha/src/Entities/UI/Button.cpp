#include "Button.h"
#include "Confirm.h"

void Button::Draw() {
    if (active and Selected(sf::Vector2i(MOUSEPOS_W_E)))
        engine.window.draw(bbox_debug);

    Entity::Draw();
    engine.window.draw(label);
    
    
    //Debug stuff
    /*
    engine.window.draw(pos_debug);
    engine.window.draw(label_debug);
    */
}

void Button::MoveBy(sf::Vector2f offset) {
    Entity::MoveBy(offset);
    Move();
}

void Button::MoveTo(sf::Vector2f new_pos) {
    Entity::MoveTo(new_pos);
    Move();
}

void Button::Move() {
    label_offset = engine.game.GetResScale();
    label.setPosition({ pos.x, pos.y - label_offset });
    label_debug.setPosition({ label.getPosition().x - 2, label.getPosition().y - 2 });
}

void Button::Pressed() {
    UI::Pressed();
}

void Button::Released() {
    activated = true;
    switch (elem) {
        case UIElems::APPLY:
            if (menu.GetLabel() == Menus::OPTIONS) {
                //Set the game's current resolution to the scale determined by the resolution picker OR set it to fullscreen if that toggle is clicked
                if (menu.GetUIElemStatus(UIElems::FULLSCREEN) == "True" and engine.game.GetResolution().x != SCREENSIZE().x)
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
            if (menu.GetLabel() == Menus::CHARCREA) {
                auto c_box = make_shared<Confirm>(
                    engine, menu,
                    AnimInfo{ "UI/ConfirmBox", 200, 100 },
                    Animation::Transform{ engine.game.camera.getCenter(), {.5f, .5f}, {engine.game.GetResScale(), engine.game.GetResScale()}},
                    UI::Style{ UIElems::CONFIRM, engine.game.GetResScale()*16},
                    "Exit Character Creation?", 1, -1);
                menu.AddUIElem(c_box); //Also adds to the current scene
                engine.game.curr_ui_layer = 1;
            }
            else {
                menu.Open(false);
                engine.scene->OpenMenu(Menus::MAIN);
            }
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
            //Spawn Name entry box and "Finish/OK" button - TO-DO
            break;
        }
        
        case UIElems::DEBUG_ROOM:
            engine.scene->CreatePreGen(PreGens::DAKN);
            engine.game.area = Areas::DEBUG;
            engine.game.SetScene(Scenes::AREA);
        break;

        case UIElems::NO:
            --engine.game.curr_ui_layer;
            menu.RemoveUIElem(UIElems::CONFIRM);
            menu.RemoveUIElem(UIElems::YES);
            alive = false;
        break;

        case UIElems::OPTIONS:
            menu.Open(false);
            engine.scene->OpenMenu(Menus::OPTIONS);
        break;

        case UIElems::QUIT:
            engine.window.close();
        break;

        case UIElems::RACE_B:
            //Close whatever other sub-menu is open - TO-DO
            //Open Race sub-menu
            menu.OpenSM(Menus::CCRACE);
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

        case UIElems::YES:
            if (menu.GetLabel() == Menus::CHARCREA) {
                //Back to layer 0
                --engine.game.curr_ui_layer;
                //Double-check to make sure this *destroys* confirm box and y/n buttons!!!!!!!! TO-DO
                menu.RemoveUIElem(UIElems::CONFIRM);
                menu.RemoveUIElem(UIElems::NO);
                alive = false;
                menu.Open(false);
                engine.scene->RemoveEntity("Default");
                engine.scene->OpenMenu(Menus::MAIN);
            }
        break;
    }
}
