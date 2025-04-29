#pragma once
#include <windows.h>
#include <winuser.h>
#include <iostream>
#include "../../Engine/Core/Vector2.h"


//Mouse buttons
constexpr int LMB = VK_LBUTTON;
constexpr int RMB = VK_RBUTTON;
constexpr int MMB = VK_MBUTTON;

//Keys
//Letters
constexpr int A_K = 'A'; //VK_A and 'A' are both acceptable
constexpr int I_K = 'I';
constexpr int O_K = 'O';
//Special
constexpr int ALT = VK_MENU;       //ALT (either left or right)
constexpr int LALT = VK_LMENU;
constexpr int RALT = VK_RMENU;
constexpr int CTRL = VK_CONTROL;   //CTRL (either left or right)
constexpr int LCTRL = VK_LCONTROL;
constexpr int RCTRL = VK_CONTROL;
constexpr int SHIFT = VK_SHIFT;
constexpr int LSHIFT = VK_LSHIFT;
constexpr int RSHIFT = VK_RSHIFT;

namespace Engine {
namespace Input {

	static HWND window = nullptr;
	static Vector2u resolution;

	//State arrays
	static BYTE prev_btns[256] = {};
	static BYTE curr_btns[256] = {};

	void Init(HWND hwnd, Vector2u res);
	void Update();
	bool BtnDown(int btn);
	bool BtnPressed(int btn);
	bool BtnReleased(int btn);

	Vector2f MousePos();
};
}