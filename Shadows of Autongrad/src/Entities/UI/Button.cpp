#include "Button.h"
//#include "Confirm.h"

void Button::Draw() {
    if (Selected())
        game->renderer.DrawRect(bbox, Color(1, 0, 0));

    Entity::Draw();

    game->renderer.DrawTxt(label);
}

void Button::Move() {
    label_offset = game->GetResScale();
    label.MoveTo({ pos.x, pos.y - label_offset });
}

void Button::Pressed() {
    UI::Pressed();
}

void Button::Released() {
    activated = true;
    switch (elem) {
    case UIElem::Apply:
        if (menu.GetLabel() == Menus::Options) {
            //Set the game's current resolution to the scale determined by the resolution picker OR set it to fullscreen if that toggle is clicked
            if (menu.GetUIElemStatus(UIElem::Fullscreen) == "True" and game->resolution != game->window.ScreenSize())
                game->SetResolution(game->window.ScreenSize());
            else {
                uint new_scale = stoi(menu.GetUIElemStatus(UIElem::Resolution));
                uint old_scale = game->GetResScale();
                if (new_scale != old_scale)
                    game->SetResolution(new_scale);
            }
            SetActive(false);
        }
        break;

    case UIElem::AS:
        //Close whatever other sub-menu is open - TO-DO
        //Open Ability Score sub-menu
        break;

    case UIElem::Back:
        //If creating a character, ask to confirm and if yes, wipe party creation
        if (menu.GetLabel() == Menus::CharCrea) {
            /*
            auto c_box = make_s<Confirm>(
                engine, menu,
                AnimInfo{ "UI/ConfirmBox", 200, 100 },
                Animation::Transform{ Vector2u(engine.game->camera.getCenter()), {.5f, .5f}, {engine.game->GetResScale(), engine.game->GetResScale()}},
                UI::Style{ UIElem::CONFIRM, engine.game->GetResScale()*16},
                "Exit Character Creation?", 1, -1);
            menu.AddUIElem(c_box); //Also adds to the current scene
            game->curr_ui_layer = 1;
            */
        }
        else {
            menu.Open(false);
            scene->OpenMenu(Menus::Main);
        }
        break;

    case UIElem::CharCrea:
        menu.Open(false);

        scene->CreatePartyMem();
        scene->OpenMenu(Menus::CharCrea);
        break;

    case UIElem::Close:
        menu.Open(false);
        scene->interface_open = Menus::NOINTRFC;
        game->paused = false;
        break;

    case UIElem::Create: {
        //Spawn Name entry box and "Finish/OK" button - TO-DO
        break;
    }

    case UIElem::Debug_Room:
        scene->CreatePreGen(PreGens::Dakn);
        game->area = Area::Debug;
        game->SetScene(Scenes::AREA);
        break;

    case UIElem::No:
        --game->curr_ui_layer;
        menu.RemoveUIElem(UIElem::Confirm);
        menu.RemoveUIElem(UIElem::Yes);
        alive = false;
        break;

    case UIElem::Options:
        menu.Open(false);
        scene->OpenMenu(Menus::Options);
        break;

    case UIElem::Quit:
        game->window.open = false;
        break;

    case UIElem::Race_B:
        //Close whatever other sub-menu is open - TO-DO
        //Open Race sub-menu
        menu.OpenSM(Menus::CCRace);
        break;

    case UIElem::Resume:
        menu.Open(false);
        game->paused = false;
        break;

    case UIElem::Title:
        menu.Open(false);
        scene->interface_open = Menus::NOINTRFC;
        game->paused = false;
        game->SetScene(Scenes::TITLE);
        break;

    case UIElem::Tutorial:
        scene->CreatePreGen(PreGens::Dakn);
        game->area = Area::Tutton;
        game->SetScene(Scenes::AREA);
        break;

    case UIElem::Yes:
        if (menu.GetLabel() == Menus::CharCrea) {
            //Back to layer 0
            --game->curr_ui_layer;
            //Double-check to make sure this *destroys* confirm box and y/n buttons!!!!!!!! TO-DO
            menu.RemoveUIElem(UIElem::Confirm);
            menu.RemoveUIElem(UIElem::No);
            alive = false;
            menu.Open(false);
            scene->RemoveEntity("Default");
            scene->OpenMenu(Menus::Main);
        }
        break;
    }
}
