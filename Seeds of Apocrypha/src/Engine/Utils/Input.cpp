#include "Input.h"

void Input::UpdateVars() {
    //Mouse
    {
        lmb_was_up = !BUTTONDOWN(LMB);
        lmb_was_down = BUTTONDOWN(LMB);
    }

    //Letters
    {
        c_was_up = !BUTTONDOWN(C_K);
        c_was_down = BUTTONDOWN(C_K);
        i_was_up = !BUTTONDOWN(I_K);
        i_was_down = BUTTONDOWN(I_K);
        j_was_up = !BUTTONDOWN(J_K);
        j_was_down = BUTTONDOWN(J_K);
        m_was_up = !BUTTONDOWN(M_K);
        m_was_down = BUTTONDOWN(M_K);
        o_was_up = !BUTTONDOWN(O_K);
        o_was_down = BUTTONDOWN(O_K);
    }

    //Special keys
    {
        tab_was_up = !BUTTONDOWN(TAB);
        tab_was_down = BUTTONDOWN(TAB);
    }
}

bool Input::KeyPressed(sf::Keyboard::Key key) {
    switch (key) {
        case TAB:
            return tab_was_up and BUTTONDOWN(TAB);

        case O_K:
            return o_was_up and BUTTONDOWN(O_K);
    }
    return false;
}

bool Input::KeyPressed(sf::Mouse::Button mb) {
    switch (mb) {
        case LMB:
            return lmb_was_up and BUTTONDOWN(LMB);
    }
    return false;
}

bool Input::KeyReleased(sf::Keyboard::Key key) {
    return false;
}

bool Input::KeyReleased(sf::Mouse::Button mb) {
    switch (mb) {
        case LMB:
            return lmb_was_down and !BUTTONDOWN(LMB);
    }
    return false;
}
