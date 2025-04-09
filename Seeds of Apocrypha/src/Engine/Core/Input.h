#pragma once
#include <iostream>

//Mouse input
#define MOUSEPOS sf::Mouse::getPosition(window)
#define MOUSEPOS_E sf::Mouse::getPosition(engine.window)
#define MOUSEPOS_W Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window)))
#define MOUSEPOS_W_E Vector2i(engine.window.mapPixelToCoords(sf::Mouse::getPosition(engine.window)))
constexpr sf::Mouse::Button LMB = sf::Mouse::Button::Left;
constexpr sf::Mouse::Button RMB = sf::Mouse::Button::Right;
constexpr sf::Mouse::Button MID = sf::Mouse::Button::Middle;
inline bool BUTTONDOWN(sf::Mouse::Button button) { return sf::Mouse::isButtonPressed(button); }

//Letters
constexpr sf::Keyboard::Key A_K = sf::Keyboard::Key::A;
constexpr sf::Keyboard::Key C_K = sf::Keyboard::Key::C;
constexpr sf::Keyboard::Key D_K = sf::Keyboard::Key::D;
constexpr sf::Keyboard::Key I_K = sf::Keyboard::Key::I;
constexpr sf::Keyboard::Key J_K = sf::Keyboard::Key::J;
constexpr sf::Keyboard::Key M_K = sf::Keyboard::Key::M;
constexpr sf::Keyboard::Key O_K = sf::Keyboard::Key::O;
constexpr sf::Keyboard::Key S_K = sf::Keyboard::Key::S;
constexpr sf::Keyboard::Key W_K = sf::Keyboard::Key::W;
//Special keys
//Other
constexpr sf::Keyboard::Key LCTRL = sf::Keyboard::Key::LControl; constexpr sf::Keyboard::Key RCTRL = sf::Keyboard::Key::RControl;
constexpr sf::Keyboard::Key LSHIFT = sf::Keyboard::Key::LShift; constexpr sf::Keyboard::Key RSHIFT = sf::Keyboard::Key::RShift;
constexpr sf::Keyboard::Key TAB = sf::Keyboard::Key::Tab;
//Arrows
constexpr sf::Keyboard::Key UP = sf::Keyboard::Key::Up;
constexpr sf::Keyboard::Key DOWN = sf::Keyboard::Key::Down;
constexpr sf::Keyboard::Key LEFT = sf::Keyboard::Key::Left;
constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::Key::Right;
inline bool BUTTONDOWN(sf::Keyboard::Key button) { return sf::Keyboard::isKeyPressed(button); }

namespace Input {
	//Variables for when keys are up vs down
	//Mouse
	static bool lmb_was_down = false;
	static bool lmb_was_up = true;
	static bool rmb_was_down = false;
	static bool rmb_was_up = true;
	static bool mid_was_down = false;
	static bool mid_was_up = true;
	//Letters
	//C
	static bool c_was_down = false;
	static bool c_was_up = true;
	//I
	static bool i_was_down = false;
	static bool i_was_up = true;
	//J
	static bool j_was_down = false;
	static bool j_was_up = true;
	//M
	static bool m_was_down = false;
	static bool m_was_up = true;
	//O
	static bool o_was_down = false;
	static bool o_was_up = true;
	//Special keys
	static bool tab_was_down = false;
	static bool tab_was_up = true;


	void UpdateVars();

	//Key pressed this frame
	bool KeyPressed(sf::Keyboard::Key key);
	bool KeyPressed(sf::Mouse::Button mb);

	//Key released this frame
	bool KeyReleased(sf::Keyboard::Key key);
	bool KeyReleased(sf::Mouse::Button mb);
}