#pragma once
#include "Menu.h"

class Interface : public Menu {
public:
	Interface(Menus init_label);

	void Update();
	void Draw() override {
		if (open) game->renderer.DrawSprite(sprite);
		Menu::Draw();
	}
private:
	Sprite sprite;
};