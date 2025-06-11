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
constexpr int B_K = SDL_SCANCODE_B;
constexpr int C_K = SDL_SCANCODE_C;
constexpr int D_K = SDL_SCANCODE_D;
constexpr int G_K = SDL_SCANCODE_G;
constexpr int I_K = SDL_SCANCODE_I;
constexpr int J_K = SDL_SCANCODE_J;
constexpr int M_K = SDL_SCANCODE_M;
constexpr int O_K = SDL_SCANCODE_O;
constexpr int S_K = SDL_SCANCODE_S;
constexpr int W_K = SDL_SCANCODE_W;
//Numbers
constexpr int SC_0 = SDL_SCANCODE_0;
constexpr int SC_1 = SDL_SCANCODE_1;
constexpr int SC_2 = SDL_SCANCODE_2;
constexpr int SC_3 = SDL_SCANCODE_3;
constexpr int SC_4 = SDL_SCANCODE_4;
constexpr int SC_5 = SDL_SCANCODE_5;
constexpr int SC_6 = SDL_SCANCODE_6;
constexpr int SC_7 = SDL_SCANCODE_7;
constexpr int SC_8 = SDL_SCANCODE_8;
constexpr int SC_9 = SDL_SCANCODE_9;
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