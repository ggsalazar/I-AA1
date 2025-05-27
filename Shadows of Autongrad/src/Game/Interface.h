#pragma once
#include "Menu.h"
#include "../Graphics/Sprite.h"

class Interface : public Menu {
public:
	Interfaces intrfc;
	Interface(Menus init_label, Interfaces init_intrfc)
		: Menu(init_label), intrfc(init_intrfc) {
		
		string sheet = "IntrfcPlaceholder";
		switch (intrfc) {
			case Interfaces::Char_Sheet:
				sheet = "IntrfcPlaceholder";
			break;

			case Interfaces::Inv:
				sheet = "IntrfcPlaceholder";
			break;

			case Interfaces::Journal:
				sheet = "IntrfcPlaceholder";
			break;

			case Interfaces::Map_Area:
				sheet = "IntrfcPlaceholder";
			break;

			case Interfaces::Map_World:
				sheet = "IntrfcPlaceholder";
			break;

			case Interfaces::Options:
				sheet = "IntrfcPlaceholder";
			break;
		}

		Sprite::Info i = {};
		i.sheet = "UI/Interfaces/" + sheet; i.frame_size = { 480, 270 }; i.origin = { .5f };
		sprite.Init(i);
	}
private:
	Sprite sprite;
};