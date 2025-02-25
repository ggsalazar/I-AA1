#include "Toggle.h"

void Toggle::Draw() {
	if (active and Selected(MOUSEPOS_E))
		engine.window.draw(bbox_debug);

	Entity::Draw();
	engine.window.draw(label);

	if (on)
		anim->SetCurrFrame(1);
	else anim->SetCurrFrame(0);
}

void Toggle::Move(sf::Vector2f offset) {
	Entity::Move(offset);
	label.setCharacterSize(engine.game.GetResScale() * 12);
	label_offset = engine.game.GetResScale() * 10;
	Text::SetOrigin(label, { 1.f, .5f });
	label.setPosition({ pos.x - label_offset, pos.y });
}

void Toggle::MoveTo(sf::Vector2f new_pos) {
	Entity::MoveTo(new_pos);
	label.setCharacterSize(engine.game.GetResScale() * 12);
	label_offset = engine.game.GetResScale() * 10;
	Text::SetOrigin(label, { 1.f, .5f });
	label.setPosition({ pos.x - label_offset, pos.y });
}

void Toggle::Released() {
	on = !on;
	//Set the Apply button to active
	menu.SetUIElemActive(UIElems::APPLY);

	menu.SetUIElemActive(UIElems::RESOLUTION, !on);
}