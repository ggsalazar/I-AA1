#include "Input.h"

void Input::Init(Window* win, Camera* cam) {
	window = win;
	camera = cam;
	curr_keys = SDL_GetKeyboardState(nullptr);
}

void Input::Update() {
	memcpy(prev_keys, curr_keys, SDL_SCANCODE_COUNT);

	curr_keys = SDL_GetKeyboardState(nullptr);

	prev_mouse_state = curr_mouse_state;
	curr_mouse_state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
}

bool Input::BtnDown(int btn) {
	return window->has_focus and curr_mouse_state & SDL_BUTTON_MASK(btn);
}

bool Input::BtnPressed(int btn) {
	return window->has_focus and curr_mouse_state & SDL_BUTTON_MASK(btn) and !(prev_mouse_state & SDL_BUTTON_MASK(btn));
}

bool Input::BtnReleased(int btn) {
	return window->has_focus and !(curr_mouse_state & SDL_BUTTON_MASK(btn)) and prev_mouse_state & SDL_BUTTON_MASK(btn);
}

Vector2i Input::MousePos() {
	return Round(mouse_pos.x + camera->viewport.x, mouse_pos.y + camera->viewport.y);
}

bool Input::KeyDown(int key) {
	return window->has_focus and curr_keys[key];
}

bool Input::KeyPressed(int key) {
	return window->has_focus and curr_keys[key] and !prev_keys[key];
}

bool Input::KeyReleased(int key) {
	return window->has_focus and !curr_keys[key] and prev_keys[key];
}