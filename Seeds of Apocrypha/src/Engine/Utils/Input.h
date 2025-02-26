#pragma once
#include <iostream>
#include "Macros.h"

namespace Input {
	//Variables for when keys are up vs down
	//Mouse
	static bool lmb_was_down = false;
	static bool lmb_was_up = true;
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