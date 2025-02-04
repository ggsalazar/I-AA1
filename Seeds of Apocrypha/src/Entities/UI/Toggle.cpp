#include "Toggle.h"

void Toggle::Draw(const bool debug) {
	if (active and Selected(MOUSEPOS))
		structure.window.draw(bbox_debug);

	Entity::Draw(debug);
	structure.window.draw(label);

	if (on)
		anim->SetCurrFrame(1);
	else anim->SetCurrFrame(0);
}

void Toggle::Move() {
	Entity::Move();
	label.setCharacterSize(structure.game.GetResScale() * 12);
	label_offset = structure.game.GetResScale() * 5;
	Text::SetOrigin(label, { 1.f, .5f });
	label.setPosition({ pos.x - label_offset, pos.y });
}

void Toggle::Released() {
	on = !on;
	//Set the Apply button to active
	menu.SetUIElemActive(UIElems::APPLY);

	menu.SetUIElemActive(UIElems::RESOLUTION, !on);
}