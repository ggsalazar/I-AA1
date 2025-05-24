#include "Toggle.h"

Toggle::Toggle(Menu& m, const Sprite::Info& s_i, const UIElem e,
	const uint init_ui_layer)
	: UI(m, s_i, e, init_ui_layer) {

	label_offset = game->GetResScale() * 10;
	label.SetOrigin({ 1.f, .5 });
	label.MoveTo({ pos.x - label_offset, pos.y });

	switch (elem) {
		case UIElem::Fullscreen:
			on = game->resolution.x == game->window.ScreenSize().x;
			SetActive(!on);
		break;

		case UIElem::Edge_Pan:
			on = game->edge_panning;
		break;
	}
}

void Toggle::Draw() {
	if (Selected())
		game->renderer.DrawRect(bbox, Color(1, 0, 0));

	Entity::Draw();
	game->renderer.DrawTxt(label);

	if (on)
		sprite.SetCurrFrame(1);
	else sprite.SetCurrFrame(0);
}

void Toggle::Move() {
	label_offset = game->GetResScale() * 10;
	label.MoveTo({ pos.x - label_offset, pos.y });
}

void Toggle::Released() {
	on = !on;

	switch (elem) {
		case UIElem::Fullscreen:
			//Set the Apply button to active
			menu.SetUIElemActive(UIElem::Apply);

			menu.SetUIElemActive(UIElem::Resolution, !on);
		break;

		case UIElem::Edge_Pan:
			game->edge_panning = on;
		break;
	}
}