#include "Interface.h"
#include "../Graphics/Sprite.h"
#include "../Entities/UI/UI.h"

Interface::Interface(Menus init_label)
	: Menu(init_label) {

	string sheet = "IntrfcPlaceholder";
	switch (label) {
        case Menus::Bestiary:
            sheet = "IntrfcPlaceholder";

            menu_text.info.str = "Bestiary";
        break;

		case Menus::Char_Sheet:
			sheet = "IntrfcPlaceholder";
            menu_text.info.str = "Character Sheet";
			break;

		case Menus::Inv:
			sheet = "IntrfcPlaceholder";
            menu_text.info.str = "Inventory";
			break;

		case Menus::Journal:
			sheet = "IntrfcPlaceholder";
            menu_text.info.str = "Journal";
			break;

		case Menus::Map_Area:
			sheet = "IntrfcPlaceholder";
            menu_text.info.str = "Area Map";
			break;

		case Menus::Map_World:
			sheet = "IntrfcPlaceholder";
            menu_text.info.str = "World Map";
			break;

		case Menus::Options:
			sheet = "IntrfcPlaceholder";
            menu_text.info.str = "Options";
			break;

        case Menus::Skill_Check:
            sheet = "IntrfcPlaceholder";
            menu_text.info.str = "Skill Check:";
            break;

        default:
            sheet = "IntrfcPlaceholder";
            break;
	}

	Sprite::Info i = {};
	i.sheet = "UI/Interfaces/" + sheet; i.frame_size = { 480, 270 }; i.origin = { .5f }; i.scale = { game->GetResScale() };
    i.dfc = 0;
	sprite.Init(i);
}

void Interface::Update() {
    Menu::Update();
	if (open) {
        sprite.MoveTo(Vector2f(game->camera.GetCenter()));
        menu_text.MoveTo(Round(game->camera.GetCenter().x, game->camera.viewport.y + game->camera.viewport.h * .2));
        sup_text.MoveTo({ menu_text.info.pos.x, menu_text.info.pos.y + 16 * game->GetResScale() });
        Vector2f elem_pos = { 0 };

        switch (label) {
            case Menus::Bestiary:
            break;

            case Menus::Char_Sheet:
            break;

            case Menus::Grimoire:
            break;

            case Menus::Inv:
            break;

            case Menus::Journal:
            break;

            case Menus::Map_Area:
            break;

            case Menus::Map_World:
            break;

            case Menus::Options_I:

                elem_pos = { (float)game->camera.GetCenter().x, game->camera.viewport.y + game->camera.viewport.h * .25f };

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

                    case UIElem::Edge_Pan:
                        elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .55f);
                        break;

                    case UIElem::Close:
                        elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .65f);
                        break;

                    case UIElem::Title:
                        elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .75f);
                        break;

                    case UIElem::Quit:
                        elem_pos.y = round(game->camera.viewport.y + game->camera.viewport.h * .85f);
                    break;
                }
                ui.second->MoveTo(elem_pos);
            }
            break;
        }
	}
}