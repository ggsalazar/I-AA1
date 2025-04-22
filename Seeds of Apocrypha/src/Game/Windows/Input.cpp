#include "Input.h"

void Engine::Input::Update() {
	memcpy(prev_btns, curr_btns, sizeof(curr_btns));

	GetKeyboardState(curr_btns);
}

bool Engine::Input::BtnDown(int btn) {
	Vector2i mp = MousePos();
	if (0 <= mp.x and mp.x <= resolution.x and 0 <= mp.y and mp.y <= resolution.y)
		return (curr_btns[btn] & 0xF0);

	return false;
}

bool Engine::Input::BtnPressed(int btn) {
	Vector2i mp = MousePos();
	if (0 <= mp.x and mp.x <= resolution.x and 0 <= mp.y and mp.y <= resolution.y)
		return (curr_btns[btn] & 0xF0) and !(prev_btns[btn] & 0xF0);

	return false;
}

bool Engine::Input::BtnReleased(int btn) {
	Vector2i mp = MousePos();
	if (0 <= mp.x and mp.x <= resolution.x and 0 <= mp.y and mp.y <= resolution.y)
		return (!curr_btns[btn] & 0xF0) and (prev_btns[btn] & 0xF0);

	return false;
}

Vector2i Engine::Input::MousePos() {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(window, &p);
	return Vector2i(p.x, p.y);
}
