#pragma once
#include "../../../Engine/Graphics/Text.h"
#include "Fonts/Font_D2D.h" //Include dwrite stuff

namespace Engine {
class Text_D2D : public Text {
public:
	Font_D2D* font; //Raw ptr is fine since Text doesn't own the Font object

	Text_D2D(Font_D2D* f, Info& i) :
		Text(i), font(f) {
		SetStr(info.str);
	}

	inline void SetCharSize(uint new_char_size) override { info.char_size = new_char_size; SetStr(info.str); }

	void SetStr(string new_str);

};
}