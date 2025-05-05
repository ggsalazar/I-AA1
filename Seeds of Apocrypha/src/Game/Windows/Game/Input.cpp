#include "Input.h"

namespace Engine {

void Input::Init(SDL_Window* win, Vector2u res) {
	window = win;
	resolution = res;
}

void Input::Update() {
	memcpy(prev_btns, curr_btns, sizeof(curr_btns));

	GetKeyboardState(curr_btns);
}

bool Input::BtnDown(int btn) {
	Vector2f mp = MousePos();
	if (0 <= mp.x and mp.x <= resolution.x and 0 <= mp.y and mp.y <= resolution.y)
		return (curr_btns[btn] & 0xF0);

	return false;
}

bool Input::BtnPressed(int btn) {
	Vector2f mp = MousePos();
	if (0 <= mp.x and mp.x <= resolution.x and 0 <= mp.y and mp.y <= resolution.y)
		return (curr_btns[btn] & 0xF0) and !(prev_btns[btn] & 0xF0);

	return false;
}

bool Input::BtnReleased(int btn) {
	Vector2f mp = MousePos();
	if (0 <= mp.x and mp.x <= resolution.x and 0 <= mp.y and mp.y <= resolution.y)
		return (!curr_btns[btn] & 0xF0) and (prev_btns[btn] & 0xF0);

	return false;
}

Vector2f Input::MousePos() {
	POINT p;
	GetCursorPos(&p);
	return Vector2f(p.x, p.y);
}

}