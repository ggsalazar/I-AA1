#include "Toggle.h"

Toggle::Toggle(const Engine& e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style,
	const uint init_ui_layer, const int init_dfc) :
	UI(e, m, a_i, t, style, init_ui_layer, init_dfc) {

	label_offset = engine.game.GetResScale() * 10;
	Text::SetOrigin(label, { 1.f, .5f });
	label.setPosition(sf::Vector2f(pos.x - label_offset, pos.y));

	on = engine.game.GetResolution().x == SCREENSIZE().x;
	active = !on;
}

void Toggle::Draw() {
	if (active and Selected(MOUSEPOS_E))
		engine.window.draw(bbox_debug);

	Entity::Draw();
	engine.window.draw(label);

	if (on)
		anim->SetCurrFrame(1);
	else anim->SetCurrFrame(0);
}

void Toggle::Move() {
	label.setCharacterSize(engine.game.GetResScale() * 12);
	label_offset = engine.game.GetResScale() * 10;
	Text::SetOrigin(label, { 1.f, .5f });
	label.setPosition(sf::Vector2f(pos.x - label_offset, pos.y));
}

void Toggle::Released() {
	on = !on;
	//Set the Apply button to active
	menu.SetUIElemActive(UIElems::APPLY);

	menu.SetUIElemActive(UIElems::RESOLUTION, !on);
}