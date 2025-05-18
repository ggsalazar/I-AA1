#include "Toggle.h"

Toggle::Toggle(Game& g, Scene* s, Menu& m, const Sprite::Info& s_i, const UIElems e,
	const uint init_ui_layer)
	: UI(g, s, m, s_i, e, init_ui_layer) {

	label_offset = game.GetResScale() * 10;
	label.SetOrigin({ 1.f, .5 });
	label.MoveTo({ pos.x - label_offset, pos.y });

	on = game.resolution.x == game.window.ScreenSize().x;
	active = !on;
}

void Toggle::Draw() {
	if (active and Selected())
		game.renderer.DrawRect(bbox, Color(0, 1, 0));

	Entity::Draw();
	game.renderer.DrawTxt(label);

	if (on)
		sprite.SetCurrFrame(1);
	else sprite.SetCurrFrame(0);
}

void Toggle::Move() {
	label_offset = game.GetResScale() * 10;
	label.MoveTo({ pos.x - label_offset, pos.y });
}

void Toggle::Released() {
	on = !on;
	//Set the Apply button to active
	menu.SetUIElemActive(UIElems::APPLY);

	menu.SetUIElemActive(UIElems::RESOLUTION, !on);
}