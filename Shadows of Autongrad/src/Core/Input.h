#pragma once
#include <SDL3/SDL_scancode.h>
#include "../Graphics/Window.h"
#include "../Core/Camera.h"
#include "Vector2.h" //iostream

//Mouse buttons
constexpr int LMB = SDL_BUTTON_LEFT;
constexpr int RMB = SDL_BUTTON_RIGHT;
constexpr int MMB = SDL_BUTTON_MIDDLE;

//Keys
//Letters
constexpr int A_K = SDL_SCANCODE_A;
constexpr int C_K = SDL_SCANCODE_C;
constexpr int D_K = SDL_SCANCODE_D;
constexpr int I_K = SDL_SCANCODE_I;
constexpr int J_K = SDL_SCANCODE_J;
constexpr int M_K = SDL_SCANCODE_M;
constexpr int O_K = SDL_SCANCODE_O;
constexpr int S_K = SDL_SCANCODE_S;
constexpr int W_K = SDL_SCANCODE_W;
//Arrows
constexpr int UP = SDL_SCANCODE_UP;
constexpr int DOWN = SDL_SCANCODE_DOWN;
constexpr int LEFT = SDL_SCANCODE_LEFT;
constexpr int RIGHT = SDL_SCANCODE_RIGHT;
//Special
constexpr int LALT = SDL_SCANCODE_LALT;
constexpr int RALT = SDL_SCANCODE_RALT;
constexpr int LCTRL = SDL_SCANCODE_LCTRL;
constexpr int RCTRL = SDL_SCANCODE_RCTRL;
constexpr int LSHIFT = SDL_SCANCODE_LSHIFT;
constexpr int RSHIFT = SDL_SCANCODE_RSHIFT;

namespace Input {

	static Window* window = nullptr;
	static Camera* camera = nullptr;

	//State arrays
	static bool prev_keys[SDL_SCANCODE_COUNT] = {0};
	static const bool* curr_keys = nullptr;
	static Uint32 prev_mouse_state = 0;
	static Uint32 curr_mouse_state = 0;
	static Vector2f mouse_pos;

	void Init(Window* win, Camera* cam);
	void Update();

	bool BtnDown(int btn);
	bool BtnPressed(int btn);
	bool BtnReleased(int btn);
	Vector2i MousePos();

	bool KeyDown(int key);
	bool KeyPressed(int key);
	bool KeyReleased(int key);
};