#include "Input.h"

void Engine::Input::Update() {
	memcpy(prev_btns, curr_btns, sizeof(curr_btns));

	GetKeyboardState(curr_btns);
}

bool Engine::Input::BtnDown(int btn) {
	return (curr_btns[btn] & 0xF0);
}

bool Engine::Input::BtnPressed(int btn) {
	return BtnDown(btn) and !(prev_btns[btn] & 0xF0);
}

bool Engine::Input::BtnReleased(int btn) {
	return !BtnDown(btn) and (prev_btns[btn] & 0xF0);
}

Vector2i Engine::Input::MousePos() {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(window, &p);
	return Vector2i(p.x, p.y);
}
