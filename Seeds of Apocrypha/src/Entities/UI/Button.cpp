#include "Button.h"
#include "Confirm.h"
#include "../../Game/Scene.h"

void Button::Draw() {
    if (active and Selected())
        game.renderer->DrawRect(bbox, Color(0,1,0));

    Entity::Draw();

    game.renderer->DrawTxt(*label);
}

void Button::Move() {
    label_offset = game.GetResScale();
    label->MoveTo({ pos.x, pos.y - label_offset });
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
                if (menu.GetUIElemStatus(UIElems::FULLSCREEN) == "True" and game.GetResolution() != game.window->ScreenSize())
                    game.SetResolution(game.window->ScreenSize());
                else {
                    uint new_scale = stoi(menu.GetUIElemStatus(UIElems::RESOLUTION));
                    uint old_scale = game.GetResScale();
                    if (new_scale != old_scale)
                        game.SetResolution(new_scale);
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
                /*
                auto c_box = make_s<Confirm>(
                    engine, menu,
                    AnimInfo{ "UI/ConfirmBox", 200, 100 },
                    Animation::Transform{ Vector2u(engine.game.camera.getCenter()), {.5f, .5f}, {engine.game.GetResScale(), engine.game.GetResScale()}},
                    UI::Style{ UIElems::CONFIRM, engine.game.GetResScale()*16},
                    "Exit Character Creation?", 1, -1);
                menu.AddUIElem(c_box); //Also adds to the current scene
                game.curr_ui_layer = 1;
                */
            }
            else {
                menu.Open(false);
                scene->OpenMenu(Menus::MAIN);
            }
        break;

        case UIElems::CHARCREA:
            menu.Open(false);
            
            scene->CreatePartyMem();
            scene->OpenMenu(Menus::CHARCREA);
        break;

        case UIElems::CLOSE:
            menu.Open(false);
            scene->interface_open = Interfaces::NONE;
            game.paused = false;
        break;

        case UIElems::CREATE: {
            //Spawn Name entry box and "Finish/OK" button - TO-DO
            break;
        }
        
        case UIElems::DEBUG_ROOM:
            scene->CreatePreGen(PreGens::DAKN);
            game.area = Areas::DEBUG;
            game.SetScene(Scenes::AREA);
        break;

        case UIElems::NO:
            --game.curr_ui_layer;
            menu.RemoveUIElem(UIElems::CONFIRM);
            menu.RemoveUIElem(UIElems::YES);
            alive = false;
        break;

        case UIElems::OPTIONS:
            menu.Open(false);
            scene->OpenMenu(Menus::OPTIONS);
        break;

        case UIElems::QUIT:
            game.window->open = false;
        break;

        case UIElems::RACE_B:
            //Close whatever other sub-menu is open - TO-DO
            //Open Race sub-menu
            menu.OpenSM(Menus::CCRACE);
        break;

        case UIElems::RESUME:
            menu.Open(false);
            game.paused = false;
        break;

        case UIElems::TITLE:
            menu.Open(false);
            game.paused = false;
            game.SetScene(Scenes::TITLE);
        break;

        case UIElems::TUTORIAL:
            scene->CreatePreGen(PreGens::DAKN);
            game.area = Areas::TUTTON;
            game.SetScene(Scenes::AREA);
        break;

        case UIElems::YES:
            if (menu.GetLabel() == Menus::CHARCREA) {
                //Back to layer 0
                --game.curr_ui_layer;
                //Double-check to make sure this *destroys* confirm box and y/n buttons!!!!!!!! TO-DO
                menu.RemoveUIElem(UIElems::CONFIRM);
                menu.RemoveUIElem(UIElems::NO);
                alive = false;
                menu.Open(false);
                scene->RemoveEntity("Default");
                scene->OpenMenu(Menus::MAIN);
            }
        break;
    }
}
